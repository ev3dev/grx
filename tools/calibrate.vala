/*
 * calibrate.vala
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
 * This file is part of the GRX graphics library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

using Grx;

public class CalibrateApplication : Grx.Application {

    const int TIMEOUT = 15; // seconds
    const int MAX_JITTER = 10; // pixels

    enum State {
        NO_DEVICES,
        TOUCH_POINT_1,
        TOUCH_POINT_2,
        TOUCH_POINT_3,
        TOUCH_POINT_4,
        SUCCESS,
        ERROR
    }

    int width;
    int height;
    int half_line_length;
    TextOptions center_options;
    int timeout;
    string? timeout_message;
    int keyboard_device_count;
    int touch_device_count;
    bool is_accumulating;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    bool waiting_for_touch;
    Point screen_points[4];
    Point touch_points[4];
    string error_message;
    DeviceManager device_manager;
    Device? device;

    public CalibrateApplication () throws GLib.Error {
        Object ();
        init ();
        device_manager = get_device_manager ();
        width = get_screen_width ();
        height = get_screen_height ();
        half_line_length = int.min (width, height) / 20;
        screen_points[0].x = half_line_length * 3;
        screen_points[0].y = screen_points[0].x;
        screen_points[1].x = width - screen_points[0].x;
        screen_points[1].y = screen_points[0].y;
        screen_points[2].x = screen_points[1].x;
        screen_points[2].y = height - screen_points[1].y;
        screen_points[3].x = screen_points[0].x;
        screen_points[3].y = screen_points[2].y;
        center_options = TextOptions () {
            font = Font.default,
            fg_color = (TextColor)Color.white,
            bg_color = (TextColor)Color.black,
            x_align = TextAlignment.CENTER
        };
    }

    public signal void key_pressed ();
    public signal void touched (TouchEvent event);

    public override void activate () {
        var cancellable = new Cancellable ();
        calibrate.begin (cancellable, (obj, res) => {
            quit ();
        });
    }

    async void calibrate (Cancellable cancellable) {

        ulong device_added_id = device_manager.device_added.connect ((device) => {
            debug ("Added: %s (%s)", device.name, device.sysname);
            if (device.has_keyboard) {
                keyboard_device_count++;
            }
            if (device.has_touch) {
                device.uncalibrate ();
                touch_device_count++;
            }
            Idle.add (calibrate.callback);
        });

        ulong device_removed_id = device_manager.device_removed.connect ((device) => {
            debug ("Removed: %s (%s)", device.name, device.sysname);
            if (device.has_keyboard) {
                keyboard_device_count--;
            }
            if (device.has_touch) {
                touch_device_count--;
            }
            Idle.add (calibrate.callback);
        });

        ulong key_pressed_id = key_pressed.connect (() => {
            debug ("Key pressed");
            cancellable.cancel ();
            Idle.add (calibrate.callback);
        });

        ulong touched_id = touched.connect ((event) => {
            switch (event.type) {
            case EventType.TOUCH_DOWN:
                waiting_for_touch = false;
                if (is_accumulating) {
                    device = event.device;
                    min_x = max_x = event.x;
                    min_y = max_y = event.y;
                    Timeout.add (500, () => {
                        is_accumulating = false;
                        calibrate.callback ();
                        return false;
                    });
                } else {
                    Idle.add (calibrate.callback);
                }
                break;
            case EventType.TOUCH_MOTION:
                if (is_accumulating) {
                    min_x = int.min (min_x, event.x);
                    min_y = int.min (min_y, event.y);
                    max_x = int.max (max_x, event.x);
                    max_y = int.max (max_y, event.y);
                }
                break;
            case EventType.TOUCH_UP:
            case EventType.TOUCH_CANCEL:
                Idle.add (calibrate.callback);
                break;
            }
        });

        Timeout.add_seconds (1, () => {
            update_inactivity_timer ();
            return true;
        });

        var state = State.NO_DEVICES;
        while (true) {
            reset_inactivity_timer ();

            switch (state) {
            case State.NO_DEVICES:
                draw_no_devices_screen ();
                break;
            case State.TOUCH_POINT_1:
                draw_touch_point_screen (0);
                break;
            case State.TOUCH_POINT_2:
                draw_touch_point_screen (1);
                break;
            case State.TOUCH_POINT_3:
                draw_touch_point_screen (2);
                break;
            case State.TOUCH_POINT_4:
                draw_touch_point_screen (3);
                break;
            case State.SUCCESS:
                draw_success_screen ();
                break;
            case State.ERROR:
                draw_error_screen ();
                break;
            }

            // wait for key press or touch
            yield;

            if (cancellable.is_cancelled ()) {
                break;
            }

            switch (state) {
            case State.NO_DEVICES:
                if (touch_device_count > 0) {
                    state = State.TOUCH_POINT_1;
                    is_accumulating = true;
                }
                break;
            case State.TOUCH_POINT_1:
                if (!is_accumulating) {
                    touch_points[0].x = (max_x + min_x) / 2;
                    touch_points[0].y = (max_y + min_y) / 2;
                    if (is_jitter_bad ()) {
                        state = State.ERROR;
                        waiting_for_touch = true;
                    } else {
                        state = State.TOUCH_POINT_2;
                        is_accumulating = true;
                        debug ("Touch point 1 min/max - x: %d/%d - y: %d/%d",
                            min_x, max_x, min_y, max_y);
                    }
                }
                break;
            case State.TOUCH_POINT_2:
                if (!is_accumulating) {
                    touch_points[1].x = (max_x + min_x) / 2;
                    touch_points[1].y = (max_y + min_y) / 2;
                    if (is_jitter_bad () || is_alignment_bad (0, 1)) {
                        state = State.ERROR;
                        waiting_for_touch = true;
                    } else {
                        state = State.TOUCH_POINT_3;
                        is_accumulating = true;
                        debug ("Touch point 2 min/max - x: %d/%d - y: %d/%d",
                            min_x, max_x, min_y, max_y);
                    }
                }
                break;
            case State.TOUCH_POINT_3:
                if (!is_accumulating) {
                    touch_points[2].x = (max_x + min_x) / 2;
                    touch_points[2].y = (max_y + min_y) / 2;
                    if (is_jitter_bad () || is_alignment_bad (1, 2)) {
                        state = State.ERROR;
                        waiting_for_touch = true;
                    } else {
                        state = State.TOUCH_POINT_4;
                        is_accumulating = true;
                        debug ("Touch point 3 min/max - x: %d/%d - y: %d/%d",
                            min_x, max_x, min_y, max_y);
                    }
                }
                break;
            case State.TOUCH_POINT_4:
                if (!is_accumulating) {
                    touch_points[3].x = (max_x + min_x) / 2;
                    touch_points[3].y = (max_y + min_y) / 2;
                    if (is_jitter_bad () || is_alignment_bad (2, 3) || is_alignment_bad (3, 0)) {
                        state = State.ERROR;
                        waiting_for_touch = true;
                    } else {
                        state = State.SUCCESS;
                        waiting_for_touch = true;
                        debug ("Touch point 4 min/max - x: %d/%d - y: %d/%d",
                            min_x, max_x, min_y, max_y);
                        debug ("screen\ttouch");
                        debug ("----------------");
                        for (int i = 0; i < 4; i++) {
                            debug ("%d,%d\t%d,%d",
                                screen_points[i].x, screen_points[i].y,
                                touch_points[i].x, touch_points[i].y);
                        }
                        var matrix = compute_matrix ();
                        output_udev_rule (matrix);
                    }
                }
                break;
            case State.SUCCESS:
                if (!waiting_for_touch) {
                    quit ();
                }
                break;
            case State.ERROR:
                if (!waiting_for_touch) {
                    if (touch_device_count > 0) {
                        state = State.TOUCH_POINT_1;
                        is_accumulating = true;
                    } else {
                        state = State.NO_DEVICES;
                    }
                }
                break;
            }
        }

        device_manager.disconnect (device_added_id);
        device_manager.disconnect (device_removed_id);
        disconnect (key_pressed_id);
        disconnect (touched_id);
    }

    /*
     * Check if there is too much jitter.
     *
     * Returns true on error (bad jitter)
     */
    bool is_jitter_bad () {
        if ((max_x - min_x) > MAX_JITTER || (max_y - min_y) > MAX_JITTER) {
            error_message = "Too much jitter";
            return true;
        }

        return false;
    }

    /*
     * check that two points are aligned on at least one axis.
     *
     * Returns true on error (bad alignment)
     */
    bool is_alignment_bad (int index1, int index2) {
        if ((touch_points[index1].x - touch_points[index2].x).abs () > MAX_JITTER &&
            (touch_points[index1].y - touch_points[index2].y).abs () > MAX_JITTER) {

            error_message = "Points are not aligned";
            return true;
        }

        return false;
    }

    void draw_no_devices_screen () {
        clear_context (Color.black);

        draw_string_with_text_options ("No devices found",
            width / 2, height / 2 - 32, center_options);
        draw_string_with_text_options ("Please connect a device",
            width / 2, height / 2 - 16, center_options);
        if (keyboard_device_count > 0) {
            draw_string_with_text_options ("Press any key to cancel",
                width / 2, height / 2 + 32, center_options);
        }
    }

    void draw_touch_point_screen (int index) {
        clear_context (Color.black);

        draw_string_with_text_options ("Touch cross-hairs to calibrate",
            width / 2, height / 2 - 16, center_options);
        if (keyboard_device_count > 0) {
            draw_string_with_text_options ("Press any key to cancel",
                width / 2, height / 2 + 16, center_options);
        }

        draw_hline (screen_points[index].x - half_line_length,
            screen_points[index].x + half_line_length,
            screen_points[index].y, Color.white);
        draw_vline (screen_points[index].x,
            screen_points[index].y - half_line_length,
            screen_points[index].y + half_line_length,
            Color.white.to_xor_mode ());
    }

    void draw_success_screen () {
        clear_context (Color.black);

        draw_string_with_text_options ("Calibration was successful",
            width / 2, height / 2 - 16, center_options);
        draw_string_with_text_options ("Touch to quit",
            width / 2, height / 2 + 16, center_options);
    }

    void draw_error_screen () {
        clear_context (Color.black);

        draw_string_with_text_options ("Calibration failed",
            width / 2, height / 2 - 32, center_options);
        draw_string_with_text_options ("%s".printf (error_message),
            width / 2, height / 2 - 16, center_options);
        draw_string_with_text_options ("Touch to try again",
            width / 2, height / 2 + 16, center_options);
        if (keyboard_device_count > 0) {
            draw_string_with_text_options ("Press any key to cancel",
                width / 2, height / 2 + 32, center_options);
        }
    }

    void clear_timeout_text () {
        if (timeout_message == null) {
            // nothing to clear yet
            return;
        }

        var string_width = center_options.font.get_string_width (timeout_message);
        var string_height = center_options.font.get_string_height (timeout_message);

        draw_filled_box (width / 2 - string_width / 2, height - 16,
                         width / 2 + string_width / 2, height - 16 + string_height,
                         center_options.bg_color.as_color);
    }

    void update_inactivity_timer () {
        timeout--;

        if (timeout == 0) {
            quit ();
            return;
        }

        if (timeout <= 10) {
            clear_timeout_text ();
            timeout_message = "Exiting in %d".printf (timeout);
            draw_string_with_text_options (timeout_message, width / 2,
                height - 16, center_options);
        }
    }

    void reset_inactivity_timer () {
        timeout = TIMEOUT;
        clear_timeout_text ();
    }

    public override void input_event (Event event) {
        switch (event.type) {
        case EventType.KEY_UP:
            key_pressed ();
            break;
        case EventType.TOUCH_DOWN:
        case EventType.TOUCH_MOTION:
        case EventType.TOUCH_UP:
        case EventType.TOUCH_CANCEL:
            touched (event.touch);
            break;
        }
    }

    void output_udev_rule (double[] matrix) {
        stderr.printf ("Save the output below to /etc/udev/rules.d/99-libinput.rules\n");
        stderr.printf ("Then apply the changes by running:\n");
        stderr.printf ("sudo udevadm trigger --action=add --subsystem-match=input"
            + " --sysname-match=%s\n\n", device.sysname);

        stdout.printf ("# This was generated by the grx calibrate utility.\n\n");
        stdout.printf ("ACTION==\"add\", KERNEL==\"event[0-9]*\", \\\n");

        var udev_client = new GUdev.Client (null);
        var udev_device = udev_client.query_by_subsystem_and_name ("input", device.sysname);
        var keys = udev_device.get_property_keys ();
        if ("ID_VENDOR_ID" in keys && "ID_MODEL_ID" in keys) {
            stdout.printf ("ENV{ID_VENDOR_ID}==\"%s\", ENV{ID_MODEL_ID}==\"%s\", \\\n",
                udev_device.get_property ("ID_VENDOR_ID"),
                udev_device.get_property ("ID_MODEL_ID"));
            // TODO: this could be a problem if there are two devices with the
            // same IDs. We could potentially also use "ID_SERIAL if present"
            // which would solve the problem for devices that actually provide
            // a unique serial number.
        } else {
            // for devices that don't have vendor/product id, we use the path.
            // hopefully these devices have a fixed path between reboots. At
            // least for platform devices, this will hopefully be true.
            stdout.printf ("ENV{ID_PATH_TAG}==\"%s\", \\\n",
                udev_device.get_property ("ID_PATH_TAG"));
        }

        stdout.printf ("ENV{LIBINPUT_CALIBRATION_MATRIX}=\"%f %f %f %f %f %f\"\n",
            matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

    }

    double[] compute_matrix () {

        // normalize points

        var tx1 = (double)touch_points[1].x / width;
        var ty1 = (double)touch_points[1].y / height;
        var tx2 = (double)touch_points[2].x / width;
        var ty2 = (double)touch_points[2].y / height;
        var tx3 = (double)touch_points[3].x / width;
        var ty3 = (double)touch_points[3].y / height;
        var sx1 = (double)screen_points[1].x / width;
        var sy1 = (double)screen_points[1].y / height;
        var sx2 = (double)screen_points[2].x / width;
        var sy2 = (double)screen_points[2].y / height;
        var sx3 = (double)screen_points[3].x / width;
        var sy3 = (double)screen_points[3].y / height;

        // Solve for the libinput calibration matrix values a-f:
        //
        //    [ a b c ]   [ tx ]   [ sx ]
        //    [ d e f ] * [ ty ] = [ sy ]
        //    [ 0 0 1 ]   [  1 ]   [  1 ]
        //
        // This translates to:
        //
        //    [ tx1 ty1 1 ]   [ a ]   [ sx1 ]
        //    [ tx2 ty2 1 ] * [ b ] = [ sx2 ]
        //    [ tx3 ty3 1 ]   [ b ]   [ sx3 ]
        //
        //    [ tx1 ty1 1 ]   [ d ]   [ sy1 ]
        //    [ tx2 ty2 1 ] * [ e ] = [ sy2 ]
        //    [ tx3 ty3 1 ]   [ f ]   [ sy3 ]
        //
        // Using Cramer's method:
        //
        //     | sx1 ty1 1 |      | tx1 sx1 1 |      | tx1 ty1 sx1 |
        //     | sx2 ty2 1 |      | tx2 sx2 1 |      | tx2 ty2 sx2 |
        //     | sx3 ty3 1 |      | tx3 sx3 1 |      | tx3 ty3 sx3 |
        // a = -------------, b = -------------, c = ---------------
        //     | tx1 ty1 1 |      | tx1 ty1 1 |      | tx1 ty1   1 |
        //     | tx2 ty2 1 |      | tx2 ty2 1 |      | tx2 ty2   1 |
        //     | tx3 ty3 1 |      | tx3 ty3 1 |      | tx3 ty3   1 |
        //
        //     | sy1 ty1 1 |      | tx1 sy1 1 |      | tx1 ty1 sy1 |
        //     | sy2 ty2 1 |      | tx2 sy2 1 |      | tx2 ty2 sy2 |
        //     | sy3 ty3 1 |      | tx3 sy3 1 |      | tx3 ty3 sy3 |
        // d = -------------, e = -------------, f = ---------------
        //     | tx1 ty1 1 |      | tx1 ty1 1 |      | tx1 ty1   1 |
        //     | tx2 ty2 1 |      | tx2 ty2 1 |      | tx2 ty2   1 |
        //     | tx3 ty3 1 |      | tx3 ty3 1 |      | tx3 ty3   1 |

        var det = determinant (tx1, ty1, 1, tx2, ty2, 1, tx3, ty3, 1);

        var a = determinant (sx1, ty1, 1,   sx2, ty2, 1,   sx3, ty3, 1) / det;
        var b = determinant (tx1, sx1, 1,   tx2, sx2, 1,   tx3, sx3, 1) / det;
        var c = determinant (tx1, ty1, sx1, tx2, ty2, sx2, tx3, ty3, sx3) / det;

        var d = determinant (sy1, ty1, 1,   sy2, ty2, 1,   sy3, ty3, 1) / det;
        var e = determinant (tx1, sy1, 1,   tx2, sy2, 1,   tx3, sy3, 1) / det;
        var f = determinant (tx1, ty1, sy1, tx2, ty2, sy2, tx3, ty3, sy3) / det;

        debug ("calibration matrix:");
        debug ("[ % 0.6f % 0.6f % 0.6f ]", a, b, c);
        debug ("[ % 0.6f % 0.6f % 0.6f ]", d, e, f);
        debug ("[ % 0.6f % 0.6f % 0.6f ]", 0, 0, 1);

        return { a, b, c, d, e, f };
    }

    /*
     * Computes the determinant of a 3x3 matrix:
     *
     *    | a b c |
     *    | d e f |
     *    | g h i |
     */
    static double determinant (double a, double b, double c,
                               double d, double e, double f,
                               double g, double h, double i)
    {
        return a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h;
    }
}

public static int main (string [] argv) {
    try {
        var app = new CalibrateApplication ();
        // TODO: this will return exit status of 0 even if the calibration
        // failed/the program timed out, etc. There is not way to override the
        // return value of the run method other than an early exit when using
        // command line options.
        return app.run (argv);
    } catch (GLib.Error err) {
        error ("%s", err.message);
    }
}

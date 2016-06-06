/*
 * life.vala
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

class LifeApplication : Grx.Application {
    char[,,] state;
    int active_state;
    Color[] color;
    uint source_id;

    public LifeApplication () throws Error {
        Object ();
        init ();
        state = new char[2, get_width (), get_height ()];
        color = { Color.black, Color.white };
        // handle console switching
        notify["is-active"].connect (console_active_handler);
    }

    public override void activate () {
        randomize ();
    }

    public override void input_event (Event event) {
        quit ();
    }

    void console_active_handler () {
        // Only draw when the console is active. Basically, this will pause the
        // application when we switch to a different console and resume the
        // application when we return this console.
        if (is_active) {
            source_id = Timeout.add (40, draw, Priority.DEFAULT_IDLE + 20);
        } else {
            Source.remove (source_id);
        }
    }

    void randomize () {
        active_state = 0;
        for (var y = 0; y < get_height (); y++) {
            for (var x = 0; x < get_width (); x++) {
                var alive = Random.int_range (0, 131) > 107;
                state[active_state, x, y] = alive ? 1 : 0;
                if (alive) {
                    fast_draw_pixel (x, y, color[1]);
                }
            }
        }
    }

    void update_state () {
        var width = get_width ();
        var height = get_height ();
        var next_state = 1 - active_state;
        for (var y = 0; y < height; y++) {
            for (var x = 0; x < width; x++) {
                var px = x - 1;
                if (px < 0) {
                    px += width;
                }
                var py = y - 1;
                if (py < 0) {
                    py += height;
                }
                var nx = x + 1;
                if (nx >= width) {
                    nx -= width;
                }
                var ny = y + 1;
                if (ny >= height) {
                    ny -= height;
                }
                var live_count = (state[active_state, px, py] +
                                  state[active_state, x,  py] +
                                  state[active_state, nx, py] +
                                  state[active_state, px, y ] +
                                  state[active_state, nx, y ] +
                                  state[active_state, px, ny] +
                                  state[active_state, x,  ny] +
                                  state[active_state, nx, ny]);
                state[next_state, x, y] =
                    (live_count | state[active_state, x, y]) == 3 ? 1 : 0;
            }
        }
        active_state = next_state;
    }

    bool draw () {
        update_state ();
        var prev_state = 1 - active_state;
        for (var y = 0; y < get_height (); y++) {
            for (var x = 0; x < get_width (); x++) {
                if (state[active_state, x, y] != state[prev_state, x, y]) {
                    fast_draw_pixel (x, y, color[state[active_state, x, y]]);
                }
            }
        }

        //return Source.CONTINUE;
        return true;
    }
}

static int main(string [] argv) {
    try {
        var app = new LifeApplication ();
        return app.run ();
    } catch (Error err) {
        error ("%s", err.message);
    }
}

using Grx;

static MainLoop main_loop;
static char[,,] state;
static int active_state;
static Color[] color;
static uint source_id;

static bool signal_handler () {
    main_loop.quit ();

    message ("Program stopped by signal");

    // TODO: why does this give a compiler error?
    // return Source.REMOVE;
    return false;
}

static void screen_active_handler (bool active) {
    if (active) {
        source_id = Idle.add (draw);
    } else {
        Source.remove (source_id);
    }
}

static void init () {
    main_loop = new MainLoop ();

    Unix.signal_add (Posix.SIGTERM, signal_handler);
    Unix.signal_add (Posix.SIGINT, signal_handler);
    Unix.signal_add (Posix.SIGHUP, signal_handler);

    if (!set_mode_default_graphics (true)) {
        error ("Could not set graphics mode");
    }
    state = new char[2, get_size_x (), get_size_y ()];
    color = { Color.black, Color.white };
    set_screen_active_callback (screen_active_handler);
    if (is_screen_active ()) {
        screen_active_handler (true);
    }
}

static void randomize () {
    active_state = 0;
    for (var y = 0; y < get_size_y (); y++) {
        for (var x = 0; x < get_size_x (); x++) {
            var alive = Random.int_range (0, 131) > 107;
            state[active_state, x, y] = alive ? 1 : 0;
            if (alive) {
                draw_point_nc (x, y, color[1]);
            }
        }
    }
}

static void update_state () {
    var width = get_size_x ();
    var height = get_size_y ();
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
            state[next_state, x, y] = (live_count | state[active_state, x, y]) == 3 ? 1 : 0;
        }
    }
    active_state = next_state;
}

static bool draw () {
    update_state ();
    var prev_state = 1 - active_state;
    for (var y = 0; y < get_size_y (); y++) {
        for (var x = 0; x < get_size_x (); x++) {
            if (state[active_state, x, y] != state[prev_state, x, y]) {
                draw_point_nc (x, y, color[state[active_state, x, y]]);
            }
        }
    }

    //return Source.CONTINUE;
    return true;
}

static int main(string [] argv) {
    init ();
    randomize ();

    main_loop.run ();

    return 0;
}

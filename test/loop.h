
#include <glib.h>
#include <grx-3.0.h>

typedef struct {
    GMainLoop *loop;
    GrxKey key;
} EventData;

static void run_main_loop_until_key_press_handle_event(
    GrxEvent *event, gpointer user_data)
{
    EventData *data = user_data;

    if (event->type == GRX_EVENT_TYPE_KEY_DOWN) {
        data->key = grx_event_get_keysym(event);
        g_main_loop_quit(data->loop);
    }
}

GrxKey run_main_loop_until_key_press()
{
    EventData data;

    data.loop = g_main_loop_new(NULL, FALSE);
    data.key = GRX_KEY_NO_SYMBOL;

    guint id =
        grx_event_handler_add(run_main_loop_until_key_press_handle_event, &data, NULL);
    g_main_loop_run(data.loop);
    g_source_remove(id);
    g_main_loop_unref(data.loop);
    return data.key;
}

#include <furi.h>

#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/button_menu.h>
#include <gui/modules/dialog_ex.h>

#include <storage/storage.h>
#include <notification/notification_messages.h>

#include "item.h"
#include "scenes/scenes.h"
#include "scenes/scene_items.h"

#include "quac.h"

/* generated by fbt from .png files in images folder */
#include <quac_icons.h>

App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->scene_manager = scene_manager_alloc(&app_scene_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_scene_custom_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_back_event_callback);

    // Create our UI elements
    app->btn_menu = button_menu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, SR_ButtonMenu, button_menu_get_view(app->btn_menu));

    app->dialog = dialog_ex_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SR_Dialog, dialog_ex_get_view(app->dialog));

    // Storage
    app->storage = furi_record_open(RECORD_STORAGE);

    // Notifications - for LED light access
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // initialize device items list
    app->depth = 0;
    app->selected_item = -1;

    app->items_view = item_get_items_view_from_path(app, NULL);

    return app;
}

void app_free(App* app) {
    furi_assert(app);

    item_items_view_free(app->items_view);

    view_dispatcher_remove_view(app->view_dispatcher, SR_ButtonMenu);

    button_menu_free(app->btn_menu);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_NOTIFICATION);

    free(app);
}

// FAP Entry Point
int32_t quac_app(void* p) {
    UNUSED(p);
    FURI_LOG_I(TAG, "QUAC! QUAC!");

    App* app = app_alloc();

    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, SR_Scene_Items);
    view_dispatcher_run(app->view_dispatcher);

    furi_record_close(RECORD_GUI);
    app_free(app);
    return 0;
}
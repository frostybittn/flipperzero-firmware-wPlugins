# View System

<cite>
**Referenced Files in This Document**   
- [view.h](file://applications/services/gui/view.h)
- [view.c](file://applications/services/gui/view.c)
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h)
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c)
- [view_port.h](file://applications/services/gui/view_port.h)
- [view_port.c](file://applications/services/gui/view_port.c)
- [gui.h](file://applications/services/gui/gui.h)
- [scene_manager.h](file://applications/services/gui/scene_manager.h)
- [battery_info.h](file://applications/debug/battery_test_app/views/battery_info.h)
- [view_display_test.h](file://applications/debug/display_test/view_display_test.h)
- [lfrfid_debug_view_tune.h](file://applications/debug/lfrfid_debug/views/lfrfid_debug_view_tune.h)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Core Components](#core-components)
3. [View Architecture](#view-architecture)
4. [View Dispatcher](#view-dispatcher)
5. [View Ports](#view-ports)
6. [Scene Management](#scene-management)
7. [View Implementation Patterns](#view-implementation-patterns)
8. [Input Event Handling](#input-event-handling)
9. [View Rendering and Performance](#view-rendering-and-performance)
10. [Best Practices](#best-practices)
11. [Conclusion](#conclusion)

## Introduction

The Flipper Zero GUI framework implements a sophisticated view system that manages rendering and input processing for applications. This system is built around three core components: views, view ports, and the view dispatcher, which work together to provide a flexible and efficient interface for application development. The framework also integrates with scene management to handle application state transitions and navigation. This document provides a comprehensive analysis of the view system, explaining its architecture, implementation details, and best practices for creating custom views.

**Section sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h#L1-L184)
- [view_port.h](file://applications/services/gui/view_port.h#L1-L111)

## Core Components

The Flipper Zero GUI framework's view system consists of several interconnected components that work together to manage the user interface. The primary components are views, which represent individual screens or UI elements; view ports, which serve as containers for views and handle rendering; and the view dispatcher, which manages the lifecycle and navigation between views. These components are supported by the scene manager, which handles application state transitions and complex navigation patterns. The system is designed to be modular and extensible, allowing developers to create custom views for specific application needs while maintaining consistency across the platform.

**Section sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h#L1-L184)
- [view_port.h](file://applications/services/gui/view_port.h#L1-L111)
- [gui.h](file://applications/services/gui/gui.h#L1-L152)

## View Architecture

The view architecture in the Flipper Zero GUI framework is centered around the View structure, which encapsulates the logic and rendering for a specific UI component. Each view contains callback functions for drawing, input processing, and lifecycle management. The view system supports different model types, including lock-free models for atomic data and locking models that use mutexes for thread-safe access to shared data. Views are designed to be reusable components that can be added to a view dispatcher and switched between as needed. The architecture promotes separation of concerns by keeping rendering logic separate from application state management.

```mermaid
classDiagram
class View {
+ViewOrientation orientation
+ViewDrawCallback draw_callback
+ViewInputCallback input_callback
+ViewCustomCallback custom_callback
+ViewNavigationCallback previous_callback
+ViewCallback enter_callback
+ViewCallback exit_callback
+ViewUpdateCallback update_callback
+void* context
+void* model
+ViewModelType model_type
+view_alloc() View*
+view_free(View* view) void
+view_set_draw_callback(View* view, ViewDrawCallback callback) void
+view_set_input_callback(View* view, ViewInputCallback callback) void
+view_allocate_model(View* view, ViewModelType type, size_t size) void
+view_get_model(View* view) void*
+view_commit_model(View* view, bool update) void
}
class Canvas {
+canvas_set_orientation(Canvas* canvas, CanvasOrientation orientation) void
+canvas_draw_dot(Canvas* canvas, uint8_t x, uint8_t y) void
+canvas_draw_line(Canvas* canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) void
+canvas_draw_frame(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) void
+canvas_draw_disc(Canvas* canvas, uint8_t x, uint8_t y, uint8_t radius) void
+canvas_draw_icon(Canvas* canvas, uint8_t x, uint8_t y, const Icon* icon) void
+canvas_draw_str(Canvas* canvas, uint8_t x, uint8_t y, const char* str) void
+canvas_draw_glyph(Canvas* canvas, uint8_t x, uint8_t y, uint16_t glyph) void
+canvas_draw_box(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) void
+canvas_draw_rframe(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius) void
}
class IconAnimation {
+IconAnimation* icon_animation
+icon_animation_start(IconAnimation* instance) void
+icon_animation_stop(IconAnimation* instance) void
+icon_animation_set_update_callback(IconAnimation* instance, void (*callback)(void* context), void* context) void
}
View --> Canvas : "uses for rendering"
View --> IconAnimation : "can be tied to"
View : ViewModelTypeLockFree
View : ViewModelTypeLocking
```

**Diagram sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [canvas.h](file://applications/services/gui/canvas.h)
- [icon_animation.h](file://applications/services/gui/icon_animation.h)

**Section sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [view.c](file://applications/services/gui/view.c#L1-L184)

## View Dispatcher

The view dispatcher is responsible for managing the lifecycle of views and handling navigation between them. It acts as an intermediary between the GUI system and individual views, ensuring that only one view is active at a time and that transitions between views are handled smoothly. The dispatcher maintains a collection of registered views and provides methods to switch between them. It also handles input events, forwarding them to the currently active view, and manages rendering by updating the associated view port when the active view changes. The view dispatcher can be configured with different event callbacks for custom events, navigation events, and periodic tick events, allowing applications to respond to various system events.

```mermaid
classDiagram
class ViewDispatcher {
+ViewPort* view_port
+FuriDict* views
+View* current_view
+View* ongoing_input_view
+FuriEventLoop* event_loop
+FuriMessageQueue* input_queue
+FuriMessageQueue* event_queue
+ViewDispatcherCustomEventCallback custom_event_callback
+ViewDispatcherNavigationEventCallback navigation_event_callback
+ViewDispatcherTickEventCallback tick_event_callback
+void* event_context
+uint32_t tick_period
+bool ongoing_input
+view_dispatcher_alloc() ViewDispatcher*
+view_dispatcher_free(ViewDispatcher* view_dispatcher) void
+view_dispatcher_enable_queue(ViewDispatcher* view_dispatcher) void
+view_dispatcher_add_view(ViewDispatcher* view_dispatcher, uint32_t view_id, View* view) void
+view_dispatcher_remove_view(ViewDispatcher* view_dispatcher, uint32_t view_id) void
+view_dispatcher_switch_to_view(ViewDispatcher* view_dispatcher, uint32_t view_id) void
+view_dispatcher_attach_to_gui(ViewDispatcher* view_dispatcher, Gui* gui, ViewDispatcherType type) void
+view_dispatcher_run(ViewDispatcher* view_dispatcher) void
+view_dispatcher_stop(ViewDispatcher* view_dispatcher) void
}
class Gui {
+gui_add_view_port(Gui* gui, ViewPort* view_port, GuiLayer layer) void
+gui_remove_view_port(Gui* gui, ViewPort* view_port) void
+gui_view_port_send_to_front(Gui* gui, ViewPort* view_port) void
+gui_view_port_send_to_back(Gui* gui, ViewPort* view_port) void
+gui_lock(Gui* gui) void
+gui_unlock(Gui* gui) void
+gui_update(Gui* gui) void
}
ViewDispatcher --> ViewPort : "contains"
ViewDispatcher --> Gui : "attaches to"
ViewDispatcher --> View : "manages"
ViewDispatcher : ViewDispatcherTypeDesktop
ViewDispatcher : ViewDispatcherTypeWindow
ViewDispatcher : ViewDispatcherTypeFullscreen
```

**Diagram sources**
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h#L1-L184)
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c#L1-L407)
- [gui.h](file://applications/services/gui/gui.h#L1-L152)

**Section sources**
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h#L1-L184)
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c#L1-L407)

## View Ports

View ports serve as the bridge between the GUI system and individual views, acting as containers that manage the rendering area and input events for a view. Each view port is associated with a specific layer in the GUI hierarchy, such as desktop, window, or fullscreen, which determines its stacking order and visibility. View ports can be enabled or disabled, allowing applications to control when a view is rendered. They also handle orientation changes, automatically remapping input events based on the current orientation. The view port system supports multiple view ports simultaneously, enabling complex UI layouts with overlapping elements. View ports are responsible for forwarding input events to their associated views and triggering rendering updates when needed.

```mermaid
classDiagram
class ViewPort {
+ViewPortDrawCallback draw_callback
+ViewPortInputCallback input_callback
+void* draw_callback_context
+void* input_callback_context
+ViewPortOrientation orientation
+uint8_t width
+uint8_t height
+bool is_enabled
+Gui* gui
+FuriMutex* mutex
+view_port_alloc() ViewPort*
+view_port_free(ViewPort* view_port) void
+view_port_set_width(ViewPort* view_port, uint8_t width) void
+view_port_set_height(ViewPort* view_port, uint8_t height) void
+view_port_enabled_set(ViewPort* view_port, bool enabled) void
+view_port_draw_callback_set(ViewPort* view_port, ViewPortDrawCallback callback, void* context) void
+view_port_input_callback_set(ViewPort* view_port, ViewPortInputCallback callback, void* context) void
+view_port_update(ViewPort* view_port) void
+view_port_set_orientation(ViewPort* view_port, ViewPortOrientation orientation) void
}
class Gui {
+GuiLayerDesktop
+GuiLayerWindow
+GuiLayerStatusBarTop
+GuiLayerStatusBarLeft
+GuiLayerStatusBarLeftSlim
+GuiLayerStatusBarRight
+GuiLayerStatusBarRightSlim
+GuiLayerFullscreen
+GuiLayerMAX
+gui_add_view_port(Gui* gui, ViewPort* view_port, GuiLayer layer) void
+gui_remove_view_port(Gui* gui, ViewPort* view_port) void
+gui_view_port_send_to_front(Gui* gui, ViewPort* view_port) void
+gui_view_port_send_to_back(Gui* gui, ViewPort* view_port) void
}
ViewPort --> Gui : "registers with"
ViewPort : ViewPortOrientationHorizontal
ViewPort : ViewPortOrientationHorizontalFlip
ViewPort : ViewPortOrientationVertical
ViewPort : ViewPortOrientationVerticalFlip
ViewPort : ViewPortOrientationMAX
```

**Diagram sources**
- [view_port.h](file://applications/services/gui/view_port.h#L1-L111)
- [view_port.c](file://applications/services/gui/view_port.c#L1-L258)
- [gui.h](file://applications/services/gui/gui.h#L1-L152)

**Section sources**
- [view_port.h](file://applications/services/gui/view_port.h#L1-L111)
- [view_port.c](file://applications/services/gui/view_port.c#L1-L258)

## Scene Management

The scene manager provides a higher-level abstraction for managing application states and navigation, building on top of the view system. It maintains a stack of scenes, allowing applications to implement complex navigation patterns with back navigation and state preservation. Each scene corresponds to a specific application state and is associated with a view that represents its UI. The scene manager handles transitions between scenes, calling appropriate lifecycle methods when entering or exiting a scene. It supports custom events, back events, and periodic tick events, allowing scenes to respond to user actions and system events. The scene manager also provides methods to search for previous scenes and switch to them, enabling non-linear navigation patterns.

```mermaid
classDiagram
class SceneManager {
+SceneManagerHandlers* handlers
+void* context
+uint32_t* scene_stack
+uint8_t stack_size
+uint8_t stack_capacity
+scene_manager_alloc(const SceneManagerHandlers* app_scene_handlers, void* context) SceneManager*
+scene_manager_free(SceneManager* scene_manager) void
+scene_manager_next_scene(SceneManager* scene_manager, uint32_t next_scene_id) void
+scene_manager_previous_scene(SceneManager* scene_manager) bool
+scene_manager_handle_custom_event(SceneManager* scene_manager, uint32_t custom_event) bool
+scene_manager_handle_back_event(SceneManager* scene_manager) bool
+scene_manager_handle_tick_event(SceneManager* scene_manager) void
+scene_manager_get_current_scene(SceneManager* scene_manager) uint32_t
+scene_manager_stop(SceneManager* scene_manager) void
}
class SceneManagerHandlers {
+const AppSceneOnEnterCallback* on_enter_handlers
+const AppSceneOnEventCallback* on_event_handlers
+const AppSceneOnExitCallback* on_exit_handlers
+const uint32_t scene_num
}
class ViewDispatcher {
+view_dispatcher_switch_to_view(ViewDispatcher* view_dispatcher, uint32_t view_id) void
}
SceneManager --> SceneManagerHandlers : "uses configuration"
SceneManager --> ViewDispatcher : "controls navigation"
SceneManager : SceneManagerEventTypeCustom
SceneManager : SceneManagerEventTypeBack
SceneManager : SceneManagerEventTypeTick
```

**Diagram sources**
- [scene_manager.h](file://applications/services/gui/scene_manager.h#L1-L190)
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h#L1-L184)

**Section sources**
- [scene_manager.h](file://applications/services/gui/scene_manager.h#L1-L190)

## View Implementation Patterns

The Flipper Zero GUI framework demonstrates several common patterns for implementing custom views across different applications. These patterns include dedicated view structures that encapsulate view-specific data and functionality, factory methods for allocating and initializing views, and accessor methods for retrieving the underlying View object. Views typically follow a consistent lifecycle with allocation, configuration, and deallocation methods. The framework encourages the use of model-view separation, where application data is stored separately from the view implementation and accessed through well-defined interfaces. This approach promotes code reuse and makes views easier to test and maintain.

```mermaid
classDiagram
class BatteryInfo {
+BatteryInfoModel* model
+View* view
+battery_info_alloc() BatteryInfo*
+battery_info_free(BatteryInfo* battery_info) void
+battery_info_get_view(BatteryInfo* battery_info) View*
+battery_info_set_data(BatteryInfo* battery_info, BatteryInfoModel* data) void
}
class BatteryInfoModel {
+float vbus_voltage
+float gauge_voltage
+float gauge_current
+float gauge_temperature
+float charging_voltage
+uint8_t charge
+uint8_t health
}
class ViewDisplayTest {
+View* view
+view_display_test_alloc() ViewDisplayTest*
+view_display_test_free(ViewDisplayTest* instance) void
+view_display_test_get_view(ViewDisplayTest* instance) View*
}
class LfRfidTuneView {
+View* view
+void (*callback)(void* context)
+void* context
+lfrfid_debug_view_tune_alloc() LfRfidTuneView*
+lfrfid_debug_view_tune_free(LfRfidTuneView* tune_view) void
+lfrfid_debug_view_tune_get_view(LfRfidTuneView* tune_view) View*
+lfrfid_debug_view_tune_set_callback(LfRfidTuneView* tune_view, void (*callback)(void* context), void* context) void
}
BatteryInfo --> BatteryInfoModel : "contains"
BatteryInfo --> View : "contains"
ViewDisplayTest --> View : "contains"
LfRfidTuneView --> View : "contains"
```

**Diagram sources**
- [battery_info.h](file://applications/debug/battery_test_app/views/battery_info.h#L1-L24)
- [view_display_test.h](file://applications/debug/display_test/view_display_test.h#L1-L13)
- [lfrfid_debug_view_tune.h](file://applications/debug/lfrfid_debug/views/lfrfid_debug_view_tune.h#L1-L24)

**Section sources**
- [battery_info.h](file://applications/debug/battery_test_app/views/battery_info.h#L1-L24)
- [view_display_test.h](file://applications/debug/display_test/view_display_test.h#L1-L13)
- [lfrfid_debug_view_tune.h](file://applications/debug/lfrfid_debug/views/lfrfid_debug_view_tune.h#L1-L24)

## Input Event Handling

The view system implements a sophisticated input event handling mechanism that ensures proper delivery of user input to the active view. Input events are processed by the view dispatcher, which forwards them to the currently active view through the associated view port. The system handles input complementarity, ensuring that press and release events are properly paired and that events are not delivered out of sequence. When a view change occurs during an input sequence, the system ensures that any remaining release events are delivered to the original view to prevent state inconsistencies. The view port also handles orientation-based remapping of input keys, automatically adjusting for different screen orientations and user preferences. Custom event callbacks allow applications to handle application-specific input events that are not consumed by the active view.

```mermaid
sequenceDiagram
participant Hardware as "Input Hardware"
participant ViewPort as "ViewPort"
participant ViewDispatcher as "ViewDispatcher"
participant View as "Active View"
Hardware->>ViewPort : InputEvent (press)
ViewPort->>ViewPort : Map input based on orientation
ViewPort->>ViewDispatcher : Forward InputEvent
ViewDispatcher->>ViewDispatcher : Check input complementarity
ViewDispatcher->>View : Process InputEvent
View-->>ViewDispatcher : Return handling result
alt Event not consumed and is Back key
ViewDispatcher->>ViewDispatcher : Call previous_callback
ViewDispatcher->>ViewDispatcher : Switch to previous view
end
Hardware->>ViewPort : InputEvent (release)
ViewPort->>ViewPort : Map input based on orientation
ViewPort->>ViewDispatcher : Forward InputEvent
ViewDispatcher->>ViewDispatcher : Check input complementarity
ViewDispatcher->>View : Process InputEvent
View-->>ViewDispatcher : Return handling result
Note over ViewDispatcher,View : Input events are processed in sequence<br/>and delivered to the appropriate view
```

**Diagram sources**
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c#L243-L308)
- [view_port.c](file://applications/services/gui/view_port.c#L226-L237)

**Section sources**
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c#L243-L308)
- [view_port.c](file://applications/services/gui/view_port.c#L226-L237)

## View Rendering and Performance

The view rendering system is designed for efficiency and responsiveness, minimizing unnecessary redraws and optimizing the rendering pipeline. Views are only rendered when they are active and their view port is enabled, reducing CPU and power consumption. The system uses a lazy update mechanism, where view updates are queued and processed during the next rendering cycle rather than immediately. This approach prevents redundant rendering when multiple model changes occur in quick succession. The canvas system provides a set of optimized drawing primitives that are implemented in hardware where possible, ensuring smooth rendering performance. The framework also supports direct draw mode for applications that require maximum rendering performance, allowing them to bypass the normal view system and draw directly to the framebuffer.

```mermaid
flowchart TD
Start([Application starts]) --> AllocateView["Allocate View with view_alloc()"]
AllocateView --> ConfigureView["Configure View callbacks and model"]
ConfigureView --> AddToDispatcher["Add View to ViewDispatcher with view_dispatcher_add_view()"]
AddToDispatcher --> SwitchToView["Switch to View with view_dispatcher_switch_to_view()"]
SwitchToView --> EnterCallback["Execute View enter_callback"]
EnterCallback --> EnableViewPort["Enable ViewPort rendering"]
EnableViewPort --> RenderLoop["Rendering Loop"]
subgraph RenderingLoop
RenderLoop --> DrawCallback["Execute View draw_callback"]
DrawCallback --> GetModel["Get model with view_get_model()"]
GetModel --> RenderContent["Render content to Canvas"]
RenderContent --> CommitModel["Commit model with view_commit_model()"]
CommitModel --> UpdateViewPort["Update ViewPort with view_port_update()"]
UpdateViewPort --> RenderToScreen["Render to screen via GUI system"]
RenderToScreen --> WaitForNextUpdate["Wait for next update"]
WaitForNextUpdate --> CheckForUpdates{"Model changed?"}
CheckForUpdates --> |Yes| DrawCallback
CheckForUpdates --> |No| WaitForNextUpdate
end
subgraph ModelUpdate
direction LR
AppLogic["Application logic changes model"] --> LockModel["Lock model with with_view_model()"]
LockModel --> ModifyModel["Modify model data"]
ModifyModel --> CommitChanges["Commit changes with update=true"]
CommitChanges --> TriggerUpdate["Trigger view_update_callback"]
TriggerUpdate --> UpdateViewPort
end
RenderLoop --> HandleInput["Handle input events"]
HandleInput --> InputCallback["Execute View input_callback"]
InputCallback --> ProcessInput["Process input and potentially modify model"]
ProcessInput --> CommitModel
```

**Diagram sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c#L236-L241)
- [view_port.c](file://applications/services/gui/view_port.c#L206-L224)

**Section sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [view_dispatcher.c](file://applications/services/gui/view_dispatcher.c#L236-L241)
- [view_port.c](file://applications/services/gui/view_port.c#L206-L224)

## Best Practices

When implementing views in the Flipper Zero GUI framework, several best practices should be followed to ensure optimal performance and user experience. First, views should minimize their model size and complexity, as larger models require more memory and processing time to lock and unlock. For frequently updated data, consider using the ViewModelTypeLockFree model type to avoid the overhead of mutex operations. Views should also be designed to handle partial updates efficiently, only redrawing the portions of the screen that have changed. When implementing input handling, views should consume only the events they can handle and allow others to propagate to higher-level handlers. For complex UIs, consider using the scene manager to organize related views and manage navigation state. Finally, views should properly clean up resources in their exit callbacks to prevent memory leaks and ensure smooth transitions.

**Section sources**
- [view.h](file://applications/services/gui/view.h#L1-L238)
- [view_dispatcher.h](file://applications/services/gui/view_dispatcher.h#L1-L184)
- [scene_manager.h](file://applications/services/gui/scene_manager.h#L1-L190)

## Conclusion

The Flipper Zero GUI framework's view system provides a robust and flexible foundation for building user interfaces on embedded devices. By separating concerns between views, view ports, and the view dispatcher, the system enables efficient rendering and input processing while maintaining a clean architectural separation. The integration with scene management allows for complex navigation patterns and state management, making it suitable for a wide range of applications. The framework's design emphasizes performance and resource efficiency, which is critical for battery-powered devices with limited processing power. By following the established patterns and best practices, developers can create responsive and intuitive user interfaces that provide a consistent experience across different applications on the platform.
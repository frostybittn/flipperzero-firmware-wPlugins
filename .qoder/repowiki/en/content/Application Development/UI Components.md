# UI Components

<cite>
**Referenced Files in This Document**   
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c)
- [battery_test_app.c](file://applications/debug/battery_test_app/battery_test_app.c)
- [display_test.c](file://applications/debug/display_test/display_test.c)
- [view_display_test.c](file://applications/debug/display_test/view_display_test.c)
- [bt_debug_app.c](file://applications/debug/bt_debug_app/bt_debug_app.c)
- [ccid_test_app.c](file://applications/debug/ccid_test/ccid_test_app.c)
</cite>

## Table of Contents
1. [Introduction](#introduction)
2. [Core UI Components Overview](#core-ui-components-overview)
3. [Menu System Implementation](#menu-system-implementation)
4. [Dialogs and Popups](#dialogs-and-popups)
5. [Text Input Components](#text-input-components)
6. [Custom Drawing and Direct Rendering](#custom-drawing-and-direct-rendering)
7. [Theming and Visual Consistency](#theming-and-visual-consistency)
8. [Layout and Interaction Best Practices](#layout-and-interaction-best-practices)
9. [Common Issues and Solutions](#common-issues-and-solutions)
10. [Application-Specific UI Examples](#application-specific-ui-examples)

## Introduction
The Flipper Zero firmware provides a comprehensive UI framework designed for embedded applications with limited screen real estate. This document details the available UI components including menus, dialogs, text inputs, popups, and custom drawing elements. The framework is built around a scene-based navigation system and leverages a modular view architecture to maintain consistency across applications while allowing for customization.

The UI components are designed to work within the constraints of the Flipper Zero's 128x64 monochrome display, prioritizing clarity, accessibility, and efficient user interaction. The system supports various input methods including button presses and infrared remote control, requiring careful consideration of user experience design.

## Core UI Components Overview

The Flipper Zero UI framework consists of several key component types that form the foundation of all applications:

- **Menus**: Hierarchical navigation interfaces for selecting options
- **Dialogs**: Modal windows for displaying information or requesting user confirmation
- **Popups**: Temporary notifications and status indicators
- **Text Inputs**: Components for collecting user input, including virtual keyboards
- **Custom Views**: Direct drawing interfaces for specialized visualizations
- **Information Displays**: Components for showing read-only data

These components are implemented through a combination of view controllers, scene managers, and input handlers that work together to create responsive user interfaces.

**Section sources**
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c#L1-L100)
- [display_test.c](file://applications/debug/display_test/display_test.c#L1-L50)

## Menu System Implementation

The menu system in Flipper Zero applications provides hierarchical navigation through application features. Menus are typically implemented using the `submenu` view component, which allows for organized presentation of options with icons and text labels.

```c
// Example from file_browser_app.c - Menu creation pattern
void file_browser_draw_menu(Canvas* canvas, Submenu* submenu) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    submenu_draw(canvas, submenu);
}
```

Menu items are added with identifiers that correspond to specific actions or scene transitions:

```c
submenu_add_item(
    submenu, 
    "Open File", 
    SubmenuIndexOpen, 
    file_browser_submenu_callback, 
    instance
);
```

The menu system supports:
- Icon-based item identification
- Callback-driven selection handling
- Dynamic item enabling/disabling
- Scrollable lists for large numbers of items

Menus are typically integrated with the scene manager system, where selecting a menu item triggers a scene transition with appropriate context data.

**Section sources**
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c#L150-L200)
- [bt_debug_app.c](file://applications/debug/bt_debug_app/bt_debug_app.c#L80-L120)

## Dialogs and Popups

Dialog components provide modal interaction for critical user decisions or information display. The Flipper Zero framework includes several dialog types:

- **Confirmation dialogs**: For user verification of actions
- **Information dialogs**: For displaying status or error messages
- **Progress indicators**: For showing operation status
- **Choice dialogs**: For selecting between multiple options

Dialogs are typically shown using the `dialog_ex` component, which provides a standardized interface for modal interactions:

```c
// Example dialog usage pattern
DialogEx* dialog = dialog_ex_alloc();
dialog_ex_set_header(dialog, "Confirmation", 64, 10, AlignCenter, AlignTop);
dialog_ex_set_text(dialog, "Proceed with operation?", 64, 22, AlignCenter, AlignTop);
dialog_ex_set_left_button_text(dialog, "Cancel");
dialog_ex_set_right_button_text(dialog, "OK");
```

The dialog system handles:
- Button press events and callbacks
- Automatic text wrapping and truncation
- Centered positioning on the display
- Proper cleanup and memory management

Popups are implemented as temporary overlays that appear for a specified duration or until dismissed:

```c
// Popup example from battery_test_app.c
void show_popup(BatteryTestApp* app, const char* text) {
    popup_set_text(app->popup, text, 64, 32, AlignCenter, AlignCenter);
    popup_set_icon(app->popup, 0, 12, &I_info_32x32);
    popup_set_timeout(app->popup, 1500);
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, BatteryTestViewPopup);
}
```

**Section sources**
- [battery_test_app.c](file://applications/debug/battery_test_app/battery_test_app.c#L200-L250)
- [ccid_test_app.c](file://applications/debug/ccid_test/ccid_test_app.c#L150-L180)

## Text Input Components

Text input on the Flipper Zero is handled through specialized components designed for the device's input constraints. The framework provides:

- **Virtual keyboard**: On-screen keyboard for text entry
- **Number input**: Specialized interface for numeric values
- **Text box**: Scrollable text display and editing

The text input system is implemented as a view component that can be integrated into any scene:

```c
// Text input setup example
TextInput* text_input = text_input_alloc();
text_input_set_header_text(text_input, "Enter Name");
text_input_set_result_callback(
    text_input,
    text_input_callback,
    instance,
    instance->text_buffer,
    TEXT_INPUT_BUFFER_SIZE,
    true
);
```

Key features of the text input system:
- Predictive text entry
- Character set filtering
- Input validation callbacks
- Buffer size management
- Special character support

The virtual keyboard layout is optimized for the device's button layout, with efficient navigation between characters and special symbols.

**Section sources**
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c#L300-L350)
- [display_test.c](file://applications/debug/display_test/display_test.c#L100-L120)

## Custom Drawing and Direct Rendering

For applications requiring specialized visualizations, the Flipper Zero provides direct drawing capabilities through the canvas interface. This allows for custom graphics, charts, and other visual elements.

The canvas system provides basic drawing primitives:
- Lines and shapes
- Text rendering
- Bitmap display
- Inversion and color control

```c
// Custom drawing example from view_display_test.c
void custom_drawing_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    canvas_clear(canvas);
    canvas_draw_frame(canvas, 2, 2, 124, 60);
    canvas_draw_str(canvas, 10, 20, "Custom View");
    canvas_draw_icon(canvas, 10, 30, &I_flipper_32x32);
}
```

Direct rendering is used for:
- Signal visualization in sub-GHz applications
- Battery level indicators
- Custom animation sequences
- Data plotting and charting

The system supports double buffering to prevent flicker during updates and provides timing functions for animation control.

**Section sources**
- [view_display_test.c](file://applications/debug/display_test/view_display_test.c#L50-L80)
- [display_test.c](file://applications/debug/display_test/display_test.c#L150-L180)

## Theming and Visual Consistency

The Flipper Zero UI framework maintains visual consistency through a standardized theming system. All applications follow the same visual language with:

- **Color scheme**: Monochrome display with inverted highlights
- **Typography**: Standard font sizes and styles
- **Iconography**: Consistent icon design and placement
- **Spacing**: Uniform padding and margins

The theme is enforced through the view system, which provides standardized components that automatically apply the correct styling. Developers are encouraged to use the built-in components rather than creating custom visual elements to maintain consistency.

Key theming guidelines:
- Use 8-pixel vertical spacing between elements
- Maintain 3-pixel margins around content
- Use standard icon sizes (8x8, 16x16, 32x32)
- Follow text hierarchy (header, body, caption)
- Use inverted backgrounds for selected items

The system automatically handles theme adjustments for different display conditions and power settings.

**Section sources**
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c#L400-L420)
- [battery_test_app.c](file://applications/debug/battery_test_app/battery_test_app.c#L100-L120)

## Layout and Interaction Best Practices

Designing effective UIs for the Flipper Zero requires careful consideration of the device's constraints:

### Screen Real Estate Management
- Prioritize essential information
- Use progressive disclosure for complex options
- Implement scrolling for long content
- Minimize text length with abbreviations when appropriate

### Navigation Patterns
- Use hierarchical menus for organization
- Provide clear back navigation
- Maintain consistent button mappings
- Use breadcrumbs for deep navigation

### Input Efficiency
- Minimize required button presses
- Provide keyboard shortcuts
- Implement predictive input
- Use default values when possible

### Accessibility Considerations
- Ensure sufficient contrast
- Provide audio feedback options
- Support infrared remote control
- Use clear, legible text

The framework encourages the use of the scene pattern, where each logical screen is implemented as a separate scene with well-defined entry and exit points, facilitating clean state management and navigation.

**Section sources**
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c#L500-L550)
- [bt_debug_app.c](file://applications/debug/bt_debug_app/bt_debug_app.c#L200-L230)

## Common Issues and Solutions

### Text Truncation
Due to limited screen width, long text strings often require truncation:

```c
// Solution: Use text wrapping or scrolling
void draw_wrapped_text(Canvas* canvas, const char* text, uint8_t x, uint8_t y) {
    // Implement word wrapping logic
    // or use scrollable text view
}
```

### Layout Problems
Fixed-size layouts may not accommodate varying content:

- Use dynamic layout calculations
- Implement responsive design patterns
- Test with longest expected text strings
- Provide overflow handling

### User Interaction Issues
- Ensure all functions are accessible via buttons
- Provide clear visual feedback for actions
- Implement debounce for button presses
- Use confirmation dialogs for destructive actions

### Performance Considerations
- Minimize screen updates
- Use partial refresh when possible
- Optimize drawing operations
- Cache frequently used graphics

The framework provides debugging tools like the display test application to help identify and resolve these issues during development.

**Section sources**
- [display_test.c](file://applications/debug/display_test/display_test.c#L200-L250)
- [view_display_test.c](file://applications/debug/display_test/view_display_test.c#L100-L130)

## Application-Specific UI Examples

### Archive Browser
The file browser application demonstrates several UI patterns:
- Directory navigation with breadcrumb display
- File listing with icons and names
- Context menus for file operations
- Search and filtering capabilities

Implemented using a combination of list views and submenu components with efficient scrolling for large directories.

### Desktop Settings
The settings application shows:
- Hierarchical menu structure
- Toggle switches for boolean options
- Value selectors for numeric settings
- Confirmation dialogs for critical changes

Uses a consistent layout with clear section headings and organized option grouping.

### NFC Application
The NFC application interface includes:
- Status indicators for reader state
- Information displays for detected tags
- Operation progress indicators
- Error messages with troubleshooting guidance

Features specialized views for different tag types and operation modes, with appropriate visual feedback for each state.

These examples illustrate how the core UI components can be combined to create effective, user-friendly interfaces within the device's constraints.

**Section sources**
- [file_browser_app.c](file://applications/debug/file_browser_test/file_browser_app.c#L600-L650)
- [battery_test_app.c](file://applications/debug/battery_test_app/battery_test_app.c#L300-L330)
- [bt_debug_app.c](file://applications/debug/bt_debug_app/bt_debug_app.c#L250-L280)
#ifndef MENU_ITEMS_HPP
#define MENU_ITEMS_HPP

struct MenuItems {
    struct Edit {
        inline static bool show_style_editor = false;

        static void do_style_editor();
        static void reset_styles();
    };

    struct File {
        static void do_exit();
    };

    struct Help {
        inline static bool show_resources = false;
        inline static bool show_about_window = false;

        static void do_report();
        static void do_resources(bool* show);
        static void do_about_window(bool* show);
    };

    struct Tools {
        inline static bool show_metrics = false;
        inline static bool show_item_picker = false;
        inline static bool show_stack_tool = false;
        inline static bool show_demo_window = false;

        static void do_metrics();
        static void do_item_picker();
        static void do_stack_tool();
        static void do_demo_window();
    };
};

#endif


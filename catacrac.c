#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include "assets_icons.h"

extern const uint8_t u8g2_font_helvB18_tr[];

static const NotificationSequence catacrac_sequence_tick = {
    &message_note_c6,
    &message_delay_50,
    &message_sound_off,
    NULL,
};

static const NotificationSequence catacrac_sequence_reveal = {
    &message_note_c6,
    &message_delay_50,
    &message_note_e6,
    &message_delay_50,
    &message_note_g6,
    &message_delay_100,
    &message_sound_off,
    NULL,
};

typedef struct {
    const char* word;
    const Icon* icon;
} CatacracWord;

static const CatacracWord catacrac_words[] = {
    {"GAT", &A_cat},
    {"GOS", &A_dog},
    {"OCELL", &A_bird},
    {"PEIX", &A_fish},
    {"CAVALL", &A_horse},
    {"VACA", &A_cow},
    {"PORC", &A_pig},
    {"OVELLA", &A_sheep},
    {"CONILL", &A_rabbit},
    {"GRANOTA", &A_frog},
    {"GIRAFA", &A_giraffe},
    {"GALLINA", &A_hen},
};

#define CATACRAC_WORD_COUNT (sizeof(catacrac_words) / sizeof(catacrac_words[0]))
#define CATACRAC_ANIM_PERIOD_MS 500

typedef enum {
    CatacracScreenMenu,
    CatacracScreenPlay,
    CatacracScreenCredits,
} CatacracScreen;

#define CATACRAC_MENU_PLAY 0
#define CATACRAC_MENU_SOUND 1
#define CATACRAC_MENU_CREDITS 2
#define CATACRAC_MENU_COUNT 3

typedef struct {
    CatacracScreen screen;
    uint8_t menu_index;
    bool sound_enabled;
    size_t word_index;
    uint32_t anim_frame;
    bool word_revealed;
    ViewPort* view_port;
} CatacracState;

static void catacrac_play_sound(CatacracState* state, NotificationApp* notification, const NotificationSequence* sequence) {
    if(state->sound_enabled) {
        notification_message(notification, sequence);
    }
}

static void catacrac_draw_menu(Canvas* canvas, CatacracState* state) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignTop, "CATACRAC");

    canvas_set_font(canvas, FontSecondary);
    for(uint8_t i = 0; i < CATACRAC_MENU_COUNT; i++) {
        uint8_t y = 24 + i * 13;
        char label[24];
        if(i == CATACRAC_MENU_SOUND) {
            snprintf(label, sizeof(label), "Sound: %s", state->sound_enabled ? "ON" : "OFF");
        } else if(i == CATACRAC_MENU_PLAY) {
            snprintf(label, sizeof(label), "Play");
        } else {
            snprintf(label, sizeof(label), "Credits");
        }
        if(i == state->menu_index) {
            canvas_draw_box(canvas, 24, y - 2, 80, 12);
            canvas_set_color(canvas, ColorWhite);
            canvas_draw_str_aligned(canvas, 64, y, AlignCenter, AlignTop, label);
            canvas_set_color(canvas, ColorBlack);
        } else {
            canvas_draw_str_aligned(canvas, 64, y, AlignCenter, AlignTop, label);
        }
    }
}

static void catacrac_draw_credits(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignCenter, "CATACRAC");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 28, AlignCenter, AlignCenter, "Vocabulari en catala");
    canvas_draw_str_aligned(canvas, 64, 40, AlignCenter, AlignCenter, "Fet per en Jack");
    canvas_draw_str_aligned(canvas, 64, 56, AlignCenter, AlignCenter, "Back per tornar");
}

static void catacrac_draw_play(Canvas* canvas, CatacracState* state) {
    const CatacracWord* word = &catacrac_words[state->word_index];

    uint8_t word_y = 30;
    bool show_word = state->word_revealed || !word->icon;
    if(word->icon) {
        uint32_t frame = state->anim_frame % icon_get_frame_count(word->icon);
        canvas_draw_bitmap(
            canvas,
            (128 - icon_get_width(word->icon)) / 2,
            2,
            icon_get_width(word->icon),
            icon_get_height(word->icon),
            icon_get_frame_data(word->icon, frame));
        word_y = 54;
    }

    if(show_word) {
        canvas_set_custom_u8g2_font(canvas, u8g2_font_helvB18_tr);
        canvas_draw_str_aligned(canvas, 64, word_y, AlignCenter, AlignCenter, word->word);
    } else {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, word_y, AlignCenter, AlignCenter, "OK?");
    }

    char counter[16];
    snprintf(
        counter,
        sizeof(counter),
        "%u/%u",
        (unsigned)(state->word_index + 1),
        (unsigned)CATACRAC_WORD_COUNT);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 126, 2, AlignRight, AlignTop, counter);
}

static void catacrac_draw_callback(Canvas* canvas, void* ctx) {
    CatacracState* state = ctx;

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    switch(state->screen) {
    case CatacracScreenMenu:
        catacrac_draw_menu(canvas, state);
        break;
    case CatacracScreenCredits:
        catacrac_draw_credits(canvas);
        break;
    case CatacracScreenPlay:
        catacrac_draw_play(canvas, state);
        break;
    }
}

static void catacrac_input_callback(InputEvent* input_event, void* ctx) {
    FuriMessageQueue* queue = ctx;
    furi_message_queue_put(queue, input_event, FuriWaitForever);
}

static void catacrac_anim_timer_callback(void* ctx) {
    CatacracState* state = ctx;
    state->anim_frame++;
    view_port_update(state->view_port);
}

int32_t catacrac_app(void* p) {
    UNUSED(p);

    CatacracState state = {
        .screen = CatacracScreenMenu,
        .menu_index = 0,
        .sound_enabled = true,
        .word_index = 0,
        .anim_frame = 0,
        .word_revealed = false,
        .view_port = NULL};

    FuriMessageQueue* input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    state.view_port = view_port;
    view_port_draw_callback_set(view_port, catacrac_draw_callback, &state);
    view_port_input_callback_set(view_port, catacrac_input_callback, input_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    NotificationApp* notification = furi_record_open(RECORD_NOTIFICATION);

    FuriTimer* anim_timer = furi_timer_alloc(
        catacrac_anim_timer_callback, FuriTimerTypePeriodic, &state);
    furi_timer_start(anim_timer, furi_ms_to_ticks(CATACRAC_ANIM_PERIOD_MS));

    InputEvent event;
    bool running = true;
    while(running) {
        if(furi_message_queue_get(input_queue, &event, FuriWaitForever) != FuriStatusOk) {
            continue;
        }

        if(state.screen == CatacracScreenPlay && event.key == InputKeyBack) {
            if(event.type == InputTypeLong) {
                state.screen = CatacracScreenMenu;
                view_port_update(view_port);
            }
            continue;
        }

        if(event.type != InputTypeShort && event.type != InputTypeRepeat) {
            continue;
        }

        switch(state.screen) {
        case CatacracScreenMenu:
            switch(event.key) {
            case InputKeyUp:
                state.menu_index =
                    (state.menu_index + CATACRAC_MENU_COUNT - 1) % CATACRAC_MENU_COUNT;
                break;
            case InputKeyDown:
                state.menu_index = (state.menu_index + 1) % CATACRAC_MENU_COUNT;
                break;
            case InputKeyOk:
                if(state.menu_index == CATACRAC_MENU_PLAY) {
                    state.screen = CatacracScreenPlay;
                    state.anim_frame = 0;
                    state.word_revealed = false;
                } else if(state.menu_index == CATACRAC_MENU_SOUND) {
                    state.sound_enabled = !state.sound_enabled;
                } else if(state.menu_index == CATACRAC_MENU_CREDITS) {
                    state.screen = CatacracScreenCredits;
                }
                break;
            case InputKeyBack:
                running = false;
                break;
            default:
                break;
            }
            view_port_update(view_port);
            break;

        case CatacracScreenCredits:
            if(event.key == InputKeyBack) {
                state.screen = CatacracScreenMenu;
                view_port_update(view_port);
            }
            break;

        case CatacracScreenPlay:
            switch(event.key) {
            case InputKeyRight:
                state.word_index = (state.word_index + 1) % CATACRAC_WORD_COUNT;
                state.anim_frame = 0;
                state.word_revealed = false;
                catacrac_play_sound(&state, notification, &catacrac_sequence_tick);
                break;
            case InputKeyLeft:
                state.word_index =
                    (state.word_index + CATACRAC_WORD_COUNT - 1) % CATACRAC_WORD_COUNT;
                state.anim_frame = 0;
                state.word_revealed = false;
                catacrac_play_sound(&state, notification, &catacrac_sequence_tick);
                break;
            case InputKeyOk:
                if(catacrac_words[state.word_index].icon && !state.word_revealed) {
                    state.word_revealed = true;
                    catacrac_play_sound(&state, notification, &catacrac_sequence_reveal);
                } else {
                    state.anim_frame = 0;
                    catacrac_play_sound(&state, notification, &catacrac_sequence_tick);
                }
                break;
            default:
                break;
            }
            view_port_update(view_port);
            break;
        }
    }

    furi_timer_stop(anim_timer);
    furi_timer_free(anim_timer);
    furi_record_close(RECORD_NOTIFICATION);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(input_queue);

    return 0;
}

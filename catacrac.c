#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>

extern const uint8_t u8g2_font_helvB18_tr[];

typedef struct {
    const char* word;
} CatacracWord;

static const CatacracWord catacrac_words[] = {
    {"GAT"},
    {"GOS"},
    {"OCELL"},
    {"PEIX"},
    {"CAVALL"},
    {"VACA"},
    {"PORC"},
    {"OVELLA"},
    {"CONILL"},
    {"GRANOTA"},
    {"GIRAFA"},
    {"GALLINA"},
};

#define CATACRAC_WORD_COUNT (sizeof(catacrac_words) / sizeof(catacrac_words[0]))

typedef struct {
    size_t word_index;
} CatacracState;

static void catacrac_draw_callback(Canvas* canvas, void* ctx) {
    CatacracState* state = ctx;

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    canvas_set_custom_u8g2_font(canvas, u8g2_font_helvB18_tr);
    canvas_draw_str_aligned(
        canvas, 64, 30, AlignCenter, AlignCenter, catacrac_words[state->word_index].word);

    char counter[16];
    snprintf(
        counter,
        sizeof(counter),
        "%u/%u",
        (unsigned)(state->word_index + 1),
        (unsigned)CATACRAC_WORD_COUNT);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 122, 60, AlignRight, AlignBottom, counter);
}

static void catacrac_input_callback(InputEvent* input_event, void* ctx) {
    FuriMessageQueue* queue = ctx;
    furi_message_queue_put(queue, input_event, FuriWaitForever);
}

int32_t catacrac_app(void* p) {
    UNUSED(p);

    CatacracState state = {.word_index = 0};

    FuriMessageQueue* input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, catacrac_draw_callback, &state);
    view_port_input_callback_set(view_port, catacrac_input_callback, input_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    InputEvent event;
    bool running = true;
    while(running) {
        if(furi_message_queue_get(input_queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.type == InputTypeShort || event.type == InputTypeRepeat) {
                switch(event.key) {
                case InputKeyRight:
                    state.word_index = (state.word_index + 1) % CATACRAC_WORD_COUNT;
                    view_port_update(view_port);
                    break;
                case InputKeyLeft:
                    state.word_index =
                        (state.word_index + CATACRAC_WORD_COUNT - 1) % CATACRAC_WORD_COUNT;
                    view_port_update(view_port);
                    break;
                case InputKeyOk:
                    view_port_update(view_port);
                    break;
                case InputKeyBack:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
    }

    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(input_queue);

    return 0;
}

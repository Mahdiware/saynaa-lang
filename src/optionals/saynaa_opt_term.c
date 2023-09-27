/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include "saynaa_optionals.h"
#include "saynaa_opt_term.inc"

#ifdef _WIN32
#include <fcntl.h>
#endif

#define TERM_IMPLEMENT
#include "thirdparty/term/term.h"
#undef TERM_IMPLEMENT

// A reference to the event class, to check is instance of.
static Handle* _cls_term_event = NULL;

static void _setSlotVector(VM* vm, int slot, int tmp, double x, double y) {

  if (!ImportModule(vm, "types", slot)) return;
  if (!GetAttribute(vm, slot, "Vector", slot)) return;
  if (!NewInstance(vm, slot, slot, 0, 0)) return;

  setSlotNumber(vm, tmp, x);
  if (!setAttribute(vm, slot, "x", tmp)) return;
  setSlotNumber(vm, tmp, y);
  if (!setAttribute(vm, slot, "y", tmp)) return;
}

void* _termEventNew(VM* vm) {
  term_Event* event = Realloc(vm, NULL, sizeof(term_Event));
  event->type = TERM_ET_UNKNOWN;
  return event;
}

void _termEventDelete(VM* vm, void* event) {
  Realloc(vm, event, 0);
}

function(_termEventGetter,
  "term.Event_getter() -> Var", "") {
  const char* name;
  if (!ValidateSlotString(vm, 1, &name, NULL)) return;

  term_Event* event = GetThis(vm);

  if (strcmp(name, "type") == 0) {
    setSlotNumber(vm, 0, (double)event->type);

  } else if (strcmp(name, "keycode") == 0) {
    setSlotNumber(vm, 0, (double)event->key.code);

  } else if (strcmp(name, "ascii") == 0) {
    setSlotNumber(vm, 0, (double)event->key.ascii);

  } else if (strcmp(name, "modifiers") == 0) {
    if (event->type == TERM_ET_KEY_DOWN) {
      setSlotNumber(vm, 0, (double)event->key.modifiers);
    } else {
      setSlotNumber(vm, 0, (double)event->mouse.modifiers);
    }

  } else if (strcmp(name, "button") == 0) {
    setSlotNumber(vm, 0, (double)event->mouse.button);

  } else if (strcmp(name, "pos") == 0) {
    reserveSlots(vm, 2);
    _setSlotVector(vm, 0, 1, event->mouse.pos.x, event->mouse.pos.y);

  } else if (strcmp(name, "scroll") == 0) {
    setSlotBool(vm, 0, event->mouse.scroll);
  }

}

void _registerEnums(VM* vm, Handle* term) {
  reserveSlots(vm, 1);
  setSlotHandle(vm, 0, term);

  setSlotNumber(vm, 1, TERM_KEY_UNKNOWN); setAttribute(vm, 0, "KEY_UNKNOWN", 1);
  setSlotNumber(vm, 1, TERM_KEY_0); setAttribute(vm, 0, "KEY_0", 1);
  setSlotNumber(vm, 1, TERM_KEY_1); setAttribute(vm, 0, "KEY_1", 1);
  setSlotNumber(vm, 1, TERM_KEY_2); setAttribute(vm, 0, "KEY_2", 1);
  setSlotNumber(vm, 1, TERM_KEY_3); setAttribute(vm, 0, "KEY_3", 1);
  setSlotNumber(vm, 1, TERM_KEY_4); setAttribute(vm, 0, "KEY_4", 1);
  setSlotNumber(vm, 1, TERM_KEY_5); setAttribute(vm, 0, "KEY_5", 1);
  setSlotNumber(vm, 1, TERM_KEY_6); setAttribute(vm, 0, "KEY_6", 1);
  setSlotNumber(vm, 1, TERM_KEY_7); setAttribute(vm, 0, "KEY_7", 1);
  setSlotNumber(vm, 1, TERM_KEY_8); setAttribute(vm, 0, "KEY_8", 1);
  setSlotNumber(vm, 1, TERM_KEY_9); setAttribute(vm, 0, "KEY_9", 1);
  setSlotNumber(vm, 1, TERM_KEY_A); setAttribute(vm, 0, "KEY_A", 1);
  setSlotNumber(vm, 1, TERM_KEY_B); setAttribute(vm, 0, "KEY_B", 1);
  setSlotNumber(vm, 1, TERM_KEY_C); setAttribute(vm, 0, "KEY_C", 1);
  setSlotNumber(vm, 1, TERM_KEY_D); setAttribute(vm, 0, "KEY_D", 1);
  setSlotNumber(vm, 1, TERM_KEY_E); setAttribute(vm, 0, "KEY_E", 1);
  setSlotNumber(vm, 1, TERM_KEY_F); setAttribute(vm, 0, "KEY_F", 1);
  setSlotNumber(vm, 1, TERM_KEY_G); setAttribute(vm, 0, "KEY_G", 1);
  setSlotNumber(vm, 1, TERM_KEY_H); setAttribute(vm, 0, "KEY_H", 1);
  setSlotNumber(vm, 1, TERM_KEY_I); setAttribute(vm, 0, "KEY_I", 1);
  setSlotNumber(vm, 1, TERM_KEY_J); setAttribute(vm, 0, "KEY_J", 1);
  setSlotNumber(vm, 1, TERM_KEY_K); setAttribute(vm, 0, "KEY_K", 1);
  setSlotNumber(vm, 1, TERM_KEY_L); setAttribute(vm, 0, "KEY_L", 1);
  setSlotNumber(vm, 1, TERM_KEY_M); setAttribute(vm, 0, "KEY_M", 1);
  setSlotNumber(vm, 1, TERM_KEY_N); setAttribute(vm, 0, "KEY_N", 1);
  setSlotNumber(vm, 1, TERM_KEY_O); setAttribute(vm, 0, "KEY_O", 1);
  setSlotNumber(vm, 1, TERM_KEY_P); setAttribute(vm, 0, "KEY_P", 1);
  setSlotNumber(vm, 1, TERM_KEY_Q); setAttribute(vm, 0, "KEY_Q", 1);
  setSlotNumber(vm, 1, TERM_KEY_R); setAttribute(vm, 0, "KEY_R", 1);
  setSlotNumber(vm, 1, TERM_KEY_S); setAttribute(vm, 0, "KEY_S", 1);
  setSlotNumber(vm, 1, TERM_KEY_T); setAttribute(vm, 0, "KEY_T", 1);
  setSlotNumber(vm, 1, TERM_KEY_U); setAttribute(vm, 0, "KEY_U", 1);
  setSlotNumber(vm, 1, TERM_KEY_V); setAttribute(vm, 0, "KEY_V", 1);
  setSlotNumber(vm, 1, TERM_KEY_W); setAttribute(vm, 0, "KEY_W", 1);
  setSlotNumber(vm, 1, TERM_KEY_X); setAttribute(vm, 0, "KEY_X", 1);
  setSlotNumber(vm, 1, TERM_KEY_Y); setAttribute(vm, 0, "KEY_Y", 1);
  setSlotNumber(vm, 1, TERM_KEY_Z); setAttribute(vm, 0, "KEY_Z", 1);
  setSlotNumber(vm, 1, TERM_KEY_ESC); setAttribute(vm, 0, "KEY_ESC", 1);
  setSlotNumber(vm, 1, TERM_KEY_ENTER); setAttribute(vm, 0, "KEY_ENTER", 1);
  setSlotNumber(vm, 1, TERM_KEY_SPACE); setAttribute(vm, 0, "KEY_SPACE", 1);
  setSlotNumber(vm, 1, TERM_KEY_HOME); setAttribute(vm, 0, "KEY_HOME", 1);
  setSlotNumber(vm, 1, TERM_KEY_END); setAttribute(vm, 0, "KEY_END", 1);
  setSlotNumber(vm, 1, TERM_KEY_PAGEUP); setAttribute(vm, 0, "KEY_PAGEUP", 1);
  setSlotNumber(vm, 1, TERM_KEY_PAGEDOWN); setAttribute(vm, 0, "KEY_PAGEDOWN", 1);
  setSlotNumber(vm, 1, TERM_KEY_LEFT); setAttribute(vm, 0, "KEY_LEFT", 1);
  setSlotNumber(vm, 1, TERM_KEY_UP); setAttribute(vm, 0, "KEY_UP", 1);
  setSlotNumber(vm, 1, TERM_KEY_RIGHT); setAttribute(vm, 0, "KEY_RIGHT", 1);
  setSlotNumber(vm, 1, TERM_KEY_DOWN); setAttribute(vm, 0, "KEY_DOWN", 1);
  setSlotNumber(vm, 1, TERM_KEY_INSERT); setAttribute(vm, 0, "KEY_INSERT", 1);
  setSlotNumber(vm, 1, TERM_KEY_DELETE); setAttribute(vm, 0, "KEY_DELETE", 1);
  setSlotNumber(vm, 1, TERM_KEY_BACKSPACE); setAttribute(vm, 0, "KEY_BACKSPACE", 1);
  setSlotNumber(vm, 1, TERM_KEY_TAB); setAttribute(vm, 0, "KEY_TAB", 1);
  setSlotNumber(vm, 1, TERM_KEY_F1); setAttribute(vm, 0, "KEY_F1", 1);
  setSlotNumber(vm, 1, TERM_KEY_F2); setAttribute(vm, 0, "KEY_F2", 1);
  setSlotNumber(vm, 1, TERM_KEY_F3); setAttribute(vm, 0, "KEY_F3", 1);
  setSlotNumber(vm, 1, TERM_KEY_F4); setAttribute(vm, 0, "KEY_F4", 1);
  setSlotNumber(vm, 1, TERM_KEY_F5); setAttribute(vm, 0, "KEY_F5", 1);
  setSlotNumber(vm, 1, TERM_KEY_F6); setAttribute(vm, 0, "KEY_F6", 1);
  setSlotNumber(vm, 1, TERM_KEY_F7); setAttribute(vm, 0, "KEY_F7", 1);
  setSlotNumber(vm, 1, TERM_KEY_F8); setAttribute(vm, 0, "KEY_F8", 1);
  setSlotNumber(vm, 1, TERM_KEY_F9); setAttribute(vm, 0, "KEY_F9", 1);
  setSlotNumber(vm, 1, TERM_KEY_F10); setAttribute(vm, 0, "KEY_F10", 1);
  setSlotNumber(vm, 1, TERM_KEY_F11); setAttribute(vm, 0, "KEY_F11", 1);
  setSlotNumber(vm, 1, TERM_KEY_F12); setAttribute(vm, 0, "KEY_F12", 1);

  setSlotNumber(vm, 1, TERM_MB_UNKNOWN); setAttribute(vm, 0, "BUTTON_UNKNOWN", 1);
  setSlotNumber(vm, 1, TERM_MB_LEFT); setAttribute(vm, 0, "BUTTON_LEFT", 1);
  setSlotNumber(vm, 1, TERM_MB_MIDDLE); setAttribute(vm, 0, "BUTTON_MIDDLE", 1);
  setSlotNumber(vm, 1, TERM_MB_RIGHT); setAttribute(vm, 0, "BUTTON_RIGHT", 1);

  setSlotNumber(vm, 1, TERM_MD_NONE); setAttribute(vm, 0, "MD_NONE", 1);
  setSlotNumber(vm, 1, TERM_MD_CTRL); setAttribute(vm, 0, "MD_CTRL", 1);
  setSlotNumber(vm, 1, TERM_MD_ALT); setAttribute(vm, 0, "MD_ALT", 1);
  setSlotNumber(vm, 1, TERM_MD_SHIFT); setAttribute(vm, 0, "MD_SHIFT", 1);

  setSlotNumber(vm, 1, TERM_ET_UNKNOWN); setAttribute(vm, 0, "EVENT_UNKNOWN", 1);
  setSlotNumber(vm, 1, TERM_ET_KEY_DOWN); setAttribute(vm, 0, "EVENT_KEY_DOWN", 1);
  setSlotNumber(vm, 1, TERM_ET_RESIZE); setAttribute(vm, 0, "EVENT_RESIZE", 1);
  setSlotNumber(vm, 1, TERM_ET_DOUBLE_CLICK); setAttribute(vm, 0, "EVENT_DOUBLE_CLICK", 1);
  setSlotNumber(vm, 1, TERM_ET_MOUSE_DOWN); setAttribute(vm, 0, "EVENT_MOUSE_DOWN", 1);
  setSlotNumber(vm, 1, TERM_ET_MOUSE_UP); setAttribute(vm, 0, "EVENT_MOUSE_UP", 1);
  setSlotNumber(vm, 1, TERM_ET_MOUSE_MOVE); setAttribute(vm, 0, "EVENT_MOUSE_MOVE", 1);
  setSlotNumber(vm, 1, TERM_ET_MOUSE_DRAG); setAttribute(vm, 0, "EVENT_MOUSE_DRAG", 1);
  setSlotNumber(vm, 1, TERM_ET_MOUSE_SCROLL); setAttribute(vm, 0, "EVENT_MOUSE_SCROLL", 1);

}

function(_termInit,
  "term.init(capture_events:Bool) -> Null",
  "Initialize terminal with raw mode for tui applications, set "
  "[capture_events] true to enable event handling.") {
  bool capture_events;
  if (!ValidateSlotBool(vm, 1, &capture_events)) return;
  term_init(capture_events);
}

function(_termCleanup,
  "term.cleanup() -> Null",
  "Cleanup and resotre the last terminal state.") {
  term_cleanup();
}

function(_termIsatty,
  "term.isatty() -> Bool",
  "Returns true if both stdin and stdout are tty.") {
  setSlotBool(vm, 0, term_isatty());
}

function(_termNewScreenBuffer,
  "term.new_screen_buffer() -> Null",
  "Switch to an alternative screen buffer.") {
  term_new_screen_buffer();
}

function(_termRestoreScreenBuffer,
  "term.restore_screen_buffer() -> Null",
  "Restore the alternative buffer which was created with "
  "term.new_screen_buffer()") {
  term_restore_screen_buffer();
}

function(_termGetSize,
  "term.getsize() -> types.Vector",
  "Returns the screen size.") {
  reserveSlots(vm, 2);
  term_Vec size = term_getsize();
  _setSlotVector(vm, 0, 1, size.x, size.y);
}

function(_termGetPosition,
  "term.getposition() -> types.Vector",
  "Returns the cursor position in the screen on a zero based coordinate.") {
  reserveSlots(vm, 2);
  term_Vec pos = term_getposition();
  _setSlotVector(vm, 0, 1, pos.x, pos.y);
}

function(_termSetPosition,
  "term.setposition(pos:types.Vector | {x, y}) -> Null",
  "Set cursor position at the [position] in the screen no a zero"
  "based coordinate.") {
  double x, y;

  int argc = GetArgc(vm);
  if (!CheckArgcRange(vm, argc, 1, 2)) return;

  if (argc == 1) {
    reserveSlots(vm, 3);
    if (!GetAttribute(vm, 1, "x", 2)) return;
    if (!ValidateSlotNumber(vm, 2, &x)) return;

    if (!GetAttribute(vm, 1, "y", 2)) return;
    if (!ValidateSlotNumber(vm, 2, &y)) return;
  } else {
    if (!ValidateSlotNumber(vm, 1, &x)) return;
    if (!ValidateSlotNumber(vm, 2, &y)) return;
  }

  term_Vec pos = term_vec((int)x, (int)y);
  term_setposition(pos);
}

function(_termReadEvent,
  "term.read_event(event:term.Event) -> Bool",
  "Read an event and update the argument [event] and return true."
  "If no event was read it'll return false.") {
  reserveSlots(vm, 3);
  setSlotHandle(vm, 2, _cls_term_event);
  if (!ValidateSlotInstanceOf(vm, 1, 2)) return;

  term_Event* event = GetSlotNativeInstance(vm, 1);
  setSlotBool(vm, 0, term_read_event(event));
}

function(_termBinaryMode,
  "term.binary_mode() -> Null",
  "On windows it'll set stdout to binary mode, on other platforms this "
  "function won't make make any difference.") {
  #ifdef _WIN32
    (void) _setmode(_fileno(stdout), _O_BINARY);
  #endif
}

/*****************************************************************************/
/* MODULE REGISTER                                                           */
/*****************************************************************************/

void registerModuleTerm(VM* vm) {
  Handle* term = NewModule(vm, "term");

  _registerEnums(vm, term);
  REGISTER_FN(term, "init", _termInit, 1);
  REGISTER_FN(term, "cleanup", _termCleanup, 0);
  REGISTER_FN(term, "isatty", _termIsatty, 0);
  REGISTER_FN(term, "new_screen_buffer", _termNewScreenBuffer, 0);
  REGISTER_FN(term, "restore_screen_buffer", _termRestoreScreenBuffer, 0);
  REGISTER_FN(term, "getsize", _termGetSize, 0);
  REGISTER_FN(term, "getposition", _termGetPosition, 0);
  REGISTER_FN(term, "setposition", _termSetPosition, -1);
  REGISTER_FN(term, "read_event", _termReadEvent, 1);

  _cls_term_event = NewClass(vm, "Event", NULL, term,
                              _termEventNew, _termEventDelete,
  "The terminal event type, that'll be used at term.read_event function to "
  "fetch events.");

  ADD_METHOD(_cls_term_event, "_getter", _termEventGetter, 1);

  ModuleAddSource(vm, term, ext_term);
  // This is required for language server. Since we need to send '\r\n' to
  // the lsp client but windows will change '\n' to '\r\n' and it'll become
  // '\r\r\n', binary mode will prevent this.
  REGISTER_FN(term, "binary_mode", _termBinaryMode, 0);

  registerModule(vm, term);
  releaseHandle(vm, term);
}

void cleanupModuleTerm(VM* vm) {
  if (_cls_term_event) releaseHandle(vm, _cls_term_event);
}
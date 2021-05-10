#include "KW_radio.h"
#include "KW_gui.h"
#include "KW_label.h"
#include "KW_label_internal.h"
#include "KW_radio_internal.h"

KW_RadioManager * KW_CreateRadioManager() {
  KW_RadioManager * radiomanager = calloc(sizeof(KW_RadioManager), 1);
  radiomanager->current_items = 0;
  return radiomanager;
}

void KW_RadioManagerSetChecked(KW_RadioManager * manager, KW_Rect * Rect) {
  manager->selected = *Rect;
  if (manager->checked) {
    KW_SetLabelIcon(manager->checked, &manager->selected);
  }
}

void KW_RadioManagerSetUnchecked(KW_RadioManager * manager, KW_Rect * Rect) {
  int i;
  manager->unselected = *Rect;
  for (i = 0; i < manager->current_items; i++) {
    KW_SetLabelIcon(manager->items[i], &manager->unselected);
  }
  if (manager->checked) {
    KW_SetLabelIcon(manager->checked, &manager->selected);
  }
}

void MouseUpRadio(KW_Widget * widget, int b) {
  KW_RadioButton * radiobutton =
      (KW_RadioButton *) KW_GetWidgetUserData(widget);
  int i;
  if (b != 1) {
    return;
  }
  radiobutton->manager->checked = widget;
  for (i = 0; i < radiobutton->manager->current_items; i++) {
    KW_SetLabelIcon(radiobutton->manager->items[i],
                    &radiobutton->manager->unselected);
  }
  KW_SetLabelIcon(widget, &radiobutton->manager->selected);
}

void DestroyRadio(KW_Widget * widget) {
  KW_RadioButton * rb = (KW_RadioButton *) KW_GetWidgetUserData(widget);
  int              i;
  for (i = 0; i < rb->manager->current_items; i++) {
    if (rb->manager->items[i] == widget) {
      break;
    }
  }
  rb->manager->current_items--;
  if (rb->manager->current_items > 0) {
    rb->manager->items[i] = rb->manager->items[rb->manager->current_items - 1];
  }
  DestroyLabel(widget);
  free(rb);
}

KW_Widget * KW_CreateRadio(KW_GUI * gui, KW_Widget * parent,
                           KW_RadioManager * manager, const char * text,
                           int value, const KW_Rect * geometry) {
  KW_Label *       label = calloc(sizeof(KW_Label), 1);
  KW_RadioButton * radiobutton = calloc(sizeof(KW_RadioButton), 1);
  KW_Widget *      widget =
      KW_CreateWidget(gui, parent, geometry, PaintLabel, DestroyRadio, label);
  KW_SetLabelText(widget, text);
  KW_SetLabelAlignment(widget, KW_LABEL_ALIGN_LEFT, 0, KW_LABEL_ALIGN_MIDDLE,
                       0);
  KW_AddGUIFontChangedHandler(gui, LabelFontChanged, widget);
  KW_LabelDisableWrap(widget);
  KW_SetWidgetUserData(widget, radiobutton);
  KW_AddWidgetMouseUpHandler(widget, &MouseUpRadio);
  KW_SetLabelIcon(widget, &manager->unselected);
  radiobutton->manager = manager;
  radiobutton->value = value;
  manager->items[manager->current_items++] = widget;
  return widget;
}

int KW_RadioManagerGetSelected(KW_RadioManager * manager) {
  if (manager->checked) {
    KW_RadioButton * rb = KW_GetWidgetUserData(manager->checked);
    return rb->value;
  }
  return 0;
}

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/message_pump_glib_x.h"

#include <gdk/gdkx.h>
#if defined(HAVE_XINPUT2)
#include <X11/extensions/XInput2.h>
#else
#include <X11/Xlib.h>
#endif

#include "base/message_pump_glib_x_dispatch.h"

namespace {

gboolean PlaceholderDispatch(GSource* source,
                             GSourceFunc cb,
                             gpointer data) {
  return TRUE;
}

#if defined(HAVE_XINPUT2)

// Setup XInput2 select for the GtkWidget.
gboolean GtkWidgetRealizeCallback(GSignalInvocationHint* hint, guint nparams,
                                  const GValue* pvalues, gpointer data) {
  GtkWidget* widget = GTK_WIDGET(g_value_get_object(pvalues));
  GdkWindow* window = widget->window;
  base::MessagePumpGlibX* msgpump = static_cast<base::MessagePumpGlibX*>(data);

  DCHECK(window);  // TODO(sad): Remove once determined if necessary.

  // TODO(sad): Do we need to set a flag on |window| to make sure we don't
  // select for the same GdkWindow multiple times? Does it matter?
  msgpump->SetupXInput2ForXWindow(GDK_WINDOW_XID(window));

  return true;
}

// We need to capture all the GDK windows that get created, and start
// listening for XInput2 events. So we setup a callback to the 'realize'
// signal for GTK+ widgets, so that whenever the signal triggers for any
// GtkWidget, which means the GtkWidget should now have a GdkWindow, we can
// setup XInput2 events for the GdkWindow.
void SetupGtkWidgetRealizeNotifier(base::MessagePumpGlibX* msgpump) {
  guint signal_id;
  gpointer klass = g_type_class_ref(GTK_TYPE_WIDGET);

  g_signal_parse_name("realize", GTK_TYPE_WIDGET, &signal_id, NULL, FALSE);
  g_signal_add_emission_hook(signal_id, 0, GtkWidgetRealizeCallback,
      static_cast<gpointer>(msgpump), NULL);

  g_type_class_unref(klass);
}

#endif  // HAVE_XINPUT2

}  // namespace

namespace base {

MessagePumpGlibX::MessagePumpGlibX() : base::MessagePumpForUI(),
#if defined(HAVE_XINPUT2)
    xiopcode_(-1),
    masters_(),
    slaves_(),
#endif
    gdksource_(NULL),
    dispatching_event_(false),
    capture_x_events_(0),
    capture_gdk_events_(0) {
  gdk_event_handler_set(&EventDispatcherX, this, NULL);

#if defined(HAVE_XINPUT2)
  InitializeXInput2();
#endif
  InitializeEventsToCapture();
}

MessagePumpGlibX::~MessagePumpGlibX() {
}

bool MessagePumpGlibX::RunOnce(GMainContext* context, bool block) {
  GdkDisplay* gdisp = gdk_display_get_default();
  Display* display = GDK_DISPLAY_XDISPLAY(gdisp);
  if (XPending(display)) {
    XEvent xev;
    XPeekEvent(display, &xev);
    if (capture_x_events_[xev.type]
#if defined(HAVE_XINPUT2)
        && (xev.type != GenericEvent || xev.xcookie.extension == xiopcode_)
#endif
        ) {
      XNextEvent(display, &xev);

      bool processed = static_cast<MessagePumpGlibXDispatcher*>
          (GetDispatcher())->Dispatch(&xev);

      if (!processed) {
        DLOG(WARNING) << "Event (" << xev.type << ") not handled.";

        // TODO(sad): It is necessary to put back the event so that the default
        // GDK events handler can take care of it. Without this, it is
        // impossible to use the omnibox at the moment. However, this will
        // eventually be removed once the omnibox code is updated for touchui.
        XPutBackEvent(display, &xev);
        g_main_context_iteration(context, FALSE);
      }
    } else {
      // TODO(sad): A couple of extra events can still sneak in during this.
      // Those should be sent back to the X queue from the dispatcher
      // EventDispatcherX.
      g_main_context_iteration(context, FALSE);
    }
  }

  bool retvalue;
  if (gdksource_) {
    // Replace the dispatch callback of the GDK event source temporarily so that
    // it doesn't read events from X.
    gboolean (*cb)(GSource*, GSourceFunc, void*) =
        gdksource_->source_funcs->dispatch;
    gdksource_->source_funcs->dispatch = PlaceholderDispatch;

    dispatching_event_ = true;
    retvalue = g_main_context_iteration(context, block);
    dispatching_event_ = false;

    gdksource_->source_funcs->dispatch = cb;
  } else {
    retvalue = g_main_context_iteration(context, block);
  }

  return retvalue;
}

void MessagePumpGlibX::InitializeEventsToCapture(void) {
  // TODO(sad): Decide which events we want to capture and update the tables
  // accordingly.
  capture_x_events_[KeyPress] = true;
  capture_gdk_events_[GDK_KEY_PRESS] = true;

  capture_x_events_[KeyRelease] = true;
  capture_gdk_events_[GDK_KEY_RELEASE] = true;

  capture_x_events_[ButtonPress] = true;
  capture_gdk_events_[GDK_BUTTON_PRESS] = true;

  capture_x_events_[ButtonRelease] = true;
  capture_gdk_events_[GDK_BUTTON_RELEASE] = true;

  capture_x_events_[MotionNotify] = true;
  capture_gdk_events_[GDK_MOTION_NOTIFY] = true;

#if defined(HAVE_XINPUT2)
  capture_x_events_[GenericEvent] = true;
#endif
}

#if defined(HAVE_XINPUT2)
void MessagePumpGlibX::InitializeXInput2(void) {
  GdkDisplay* display = gdk_display_get_default();
  Display* xdisplay = GDK_DISPLAY_XDISPLAY(display);
  int event, err;

  if (!XQueryExtension(xdisplay, "XInputExtension", &xiopcode_, &event, &err)) {
    DLOG(WARNING) << "X Input extension not available.";
    xiopcode_ = -1;
    return;
  }

  int major = 2, minor = 0;
  if (XIQueryVersion(xdisplay, &major, &minor) == BadRequest) {
    DLOG(WARNING) << "XInput2 not supported in the server.";
    xiopcode_ = -1;
    return;
  }

  SetupGtkWidgetRealizeNotifier(this);

  // Instead of asking X for the list of devices all the time, let's maintain a
  // list of slave (physical) and master (virtual) pointer devices.
  int count = 0;
  XIDeviceInfo* devices = XIQueryDevice(xdisplay, XIAllDevices, &count);
  for (int i = 0; i < count; i++) {
    XIDeviceInfo* devinfo = devices + i;
    if (devinfo->use == XISlavePointer) {
      slaves_.insert(devinfo->deviceid);
    } else if (devinfo->use == XIMasterPointer) {
      masters_.insert(devinfo->deviceid);
    }
    // We do not need to care about XIFloatingSlave, because the callback for
    // XI_HierarchyChanged event will take care of it.
  }
  XIFreeDeviceInfo(devices);

  // TODO(sad): Select on root for XI_HierarchyChanged so that slaves_ and
  // masters_ can be kept up-to-date. This is a relatively rare event, so we can
  // put it off for a later time.
  // Note: It is not necessary to listen for XI_DeviceChanged events.
}

void MessagePumpGlibX::SetupXInput2ForXWindow(Window xwindow) {
  Display* xdisplay = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());

  // Setup mask for mouse events.
  unsigned char mask[(XI_LASTEVENT + 7)/8];
  memset(mask, 0, sizeof(mask));

  XISetMask(mask, XI_ButtonPress);
  XISetMask(mask, XI_ButtonRelease);
  XISetMask(mask, XI_Motion);

  // It is necessary to select only for the master devices. XInput2 provides
  // enough information to the event callback to decide which slave device
  // triggered the event, thus decide whether the 'pointer event' is a 'mouse
  // event' or a 'touch event'. So it is not necessary to select for the slave
  // devices here.
  XIEventMask evmasks[masters_.size()];
  int count = 0;
  for (std::set<int>::const_iterator iter = masters_.begin();
       iter != masters_.end();
       ++iter, ++count) {
    evmasks[count].deviceid = *iter;
    evmasks[count].mask_len = sizeof(mask);
    evmasks[count].mask = mask;
  }

  XISelectEvents(xdisplay, xwindow, evmasks, masters_.size());

  // TODO(sad): Setup masks for keyboard events.

  XFlush(xdisplay);
}

#endif  // HAVE_XINPUT2

void MessagePumpGlibX::EventDispatcherX(GdkEvent* event, gpointer data) {
  MessagePumpGlibX* pump_x = reinterpret_cast<MessagePumpGlibX*>(data);

  if (!pump_x->gdksource_) {
    pump_x->gdksource_ = g_main_current_source();
  } else if (!pump_x->IsDispatchingEvent()) {
    if (event->type != GDK_NOTHING &&
        pump_x->capture_gdk_events_[event->type]) {
      // TODO(sad): An X event is caught by the GDK handler. Put it back in the
      // X queue so that we catch it in the next iteration. When done, the
      // following DLOG statement will be removed.
      DLOG(WARNING) << "GDK received an event it shouldn't have";
    }
  }

  pump_x->DispatchEvents(event);
}

}  // namespace base

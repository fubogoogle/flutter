// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_LINUX_FL_BASIC_MESSAGE_CHANNEL_H_
#define FLUTTER_SHELL_PLATFORM_LINUX_FL_BASIC_MESSAGE_CHANNEL_H_

#if !defined(__FLUTTER_LINUX_INSIDE__) && !defined(FLUTTER_LINUX_COMPILATION)
#error "Only <flutter_linux/flutter_linux.h> can be included directly."
#endif

#include <gio/gio.h>
#include <glib-object.h>

#include "fl_binary_messenger.h"
#include "fl_message_codec.h"

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(FlBasicMessageChannel,
                     fl_basic_message_channel,
                     FL,
                     BASIC_MESSAGE_CHANNEL,
                     GObject)

/**
 * FlBasicMessageChannel:
 *
 * #FlBasicMessageChannel is an object that allows sending and receiving
 * messages to/from Dart code over platform channels.
 *
 * The following example shows how to send messages on a channel:
 *
 * |[<!-- language="C" -->
 * static FlBasicMessageChannel *channel = NULL;
 *
 * static void message_cb (FlBasicMessageChannel* channel,
 *                         FlValue* message,
 *                         FlBasicMessageChannelResponseHandle* response_handle,
 *                         gpointer user_data) {
 *   g_autoptr(FlValue) response = handle_message (message);
 *   g_autoptr(GError) error = NULL;
 *   if (!fl_basic_message_channel_respond (channel, response_handle, response,
 *                                          &error))
 *     g_warning ("Failed to send channel response: %s", error->message);
 * }
 *
 * static void message_response_cb (GObject *object,
 *                                  GAsyncResult *result,
 *                                  gpointer user_data) {
 *   g_autoptr(GError) error = NULL;
 *   g_autoptr(FlValue) response =
 *     fl_basic_message_channel_send_finish (FL_BASIC_MESSAGE_CHANNEL (object),
 *                                           result, &error);
 *   if (response == NULL) {
 *     g_warning ("Failed to send message: %s", error->message);
 *     return;
 *   }
 *
 *   handle_response (response);
 * }
 *
 * static void setup_channel () {
 *   g_autoptr(FlStandardMessageCodec) codec = fl_standard_message_codec_new ();
 *   channel = fl_basic_message_channel_new (messenger, "flutter/foo",
 *                                           FL_MESSAGE_CODEC (codec));
 *   fl_basic_message_channel_set_message_handler (channel, message_cb, NULL);
 *
 *   g_autoptr(FlValue) message = fl_value_new_string ("Hello World");
 *   fl_basic_message_channel_send (channel, message, NULL,
 *                                  message_response_cb, NULL);
 * }
 * ]|
 *
 * #FlBasicMessageChannel matches the BasicMessageChannel class in the Flutter
 * services library.
 */

/**
 * FlBasicMessageChannelResponseHandle:
 *
 * A handle used to respond to messages.
 */
typedef struct _FlBasicMessageChannelResponseHandle
    FlBasicMessageChannelResponseHandle;

/**
 * FlBasicMessageChannelMessageHandler:
 * @channel: an #FlBasicMessageChannel.
 * @message: message received.
 * @response_handle: (transfer full): a handle to respond to the message with.
 * @user_data: (closure): data provided when registering this handler.
 *
 * Function called when a message is received.
 */
typedef void (*FlBasicMessageChannelMessageHandler)(
    FlBasicMessageChannel* channel,
    FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle,
    gpointer user_data);

/**
 * fl_basic_message_channel_new:
 * @messenger: an #FlBinaryMessenger.
 * @name: a channel name.
 * @codec: the message codec.
 *
 * Creates a basic message channel. @codec must match the codec used on the Dart
 * end of the channel.
 *
 * Returns: a new #FlBasicMessageChannel.
 */
FlBasicMessageChannel* fl_basic_message_channel_new(
    FlBinaryMessenger* messenger,
    const gchar* name,
    FlMessageCodec* codec);

/**
 * fl_basic_message_channel_set_message_handler:
 * @channel: an #FlBasicMessageChannel.
 * @handler: (allow-none): function to call when a message is received on this
 * channel or %NULL to disable the handler.
 * @user_data: (closure): user data to pass to @handler.
 *
 * Sets the function called when a message is received.
 */
void fl_basic_message_channel_set_message_handler(
    FlBasicMessageChannel* channel,
    FlBasicMessageChannelMessageHandler handler,
    gpointer user_data);

/**
 * fl_basic_message_channel_respond:
 * @channel: an #FlBasicMessageChannel.
 * @response_handle: (transfer full): handle that was provided in a
 * #FlBasicMessageChannelMessageHandler.
 * @message: (allow-none): message response to send or %NULL for an empty
 * response.
 * @error: (allow-none): #GError location to store the error occurring, or %NULL
 * to ignore.
 *
 * Responds to a message.
 *
 * Returns: %TRUE on success.
 */
gboolean fl_basic_message_channel_respond(
    FlBasicMessageChannel* channel,
    FlBasicMessageChannelResponseHandle* response_handle,
    FlValue* message,
    GError** error);

/**
 * fl_basic_message_channel_send:
 * @channel: an #FlBasicMessageChannel.
 * @message: message to send, must match what the #FlMessageCodec supports.
 * @cancellable: (allow-none): a #GCancellable or %NULL.
 * @callback: (scope async): (allow-none): a #GAsyncReadyCallback to call when
 * the request is satisfied or %NULL to ignore the response.
 * @user_data: (closure): user data to pass to @callback.
 *
 * Asynchronously sends a message.
 */
void fl_basic_message_channel_send(FlBasicMessageChannel* channel,
                                   FlValue* message,
                                   GCancellable* cancellable,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data);

/**
 * fl_basic_message_channel_send_finish:
 * @channel: an #FlBasicMessageChannel.
 * @result: a #GAsyncResult.
 * @error: (allow-none): #GError location to store the error occurring, or %NULL
 * to ignore.
 *
 * Completes request started with fl_basic_message_channel_send().
 *
 * Returns: message response on success or %NULL on error.
 */
FlValue* fl_basic_message_channel_send_on_channel_finish(
    FlBasicMessageChannel* channel,
    GAsyncResult* result,
    GError** error);

G_END_DECLS

#endif  // FLUTTER_SHELL_PLATFORM_LINUX_FL_BASIC_MESSAGE_CHANNEL_H_

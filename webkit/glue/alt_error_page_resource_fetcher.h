// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_
#define WEBKIT_GLUE_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_

#include "base/scoped_ptr.h"
#include "base/task.h"
#include "googleurl/src/gurl.h"
#include "webkit/api/public/WebURLError.h"

class WebFrame;

namespace WebKit {
class WebURLResponse;
}

namespace webkit_glue {
class ResourceFetcherWithTimeout;

// Used for downloading alternate dns error pages. Once downloading is done
// (or fails), the webview delegate is notified.
class AltErrorPageResourceFetcher {
 public:
  // This will be called when the alternative error page has been fetched,
  // successfully or not.  If there is a failure, the third parameter (the
  // data) will be empty.
  typedef Callback3<
      WebFrame*, const WebKit::WebURLError&, const std::string&>::Type Callback;

  AltErrorPageResourceFetcher(const GURL& url,
                              WebFrame* frame,
                              const WebKit::WebURLError& original_error,
                              Callback* callback);
  ~AltErrorPageResourceFetcher();

  // Stop any pending loads.
  void Cancel();

 private:
  void OnURLFetchComplete(const WebKit::WebURLResponse& response,
                          const std::string& data);

  // Does the actual fetching.
  scoped_ptr<ResourceFetcherWithTimeout> fetcher_;

  WebFrame* frame_;
  scoped_ptr<Callback> callback_;

  // The error associated with this load.  If there's an error talking with the
  // alt error page server, we need this to complete the original load.
  WebKit::WebURLError original_error_;

  DISALLOW_COPY_AND_ASSIGN(AltErrorPageResourceFetcher);
};

}  // namespace webkit_glue

#endif  // WEBKIT_GLUE_ALT_ERROR_PAGE_RESOURCE_FETCHER_H_

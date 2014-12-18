// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_SHARED_WORKER_DEVTOOLS_AGENT_HOST_H_
#define CONTENT_BROWSER_DEVTOOLS_SHARED_WORKER_DEVTOOLS_AGENT_HOST_H_

#include "content/browser/devtools/embedded_worker_devtools_agent_host.h"

namespace content {

class SharedWorkerInstance;

class SharedWorkerDevToolsAgentHost : public EmbeddedWorkerDevToolsAgentHost {
 public:
  SharedWorkerDevToolsAgentHost(WorkerId worker_id,
                                const SharedWorkerInstance& shared_worker);

  // DevToolsAgentHost override.
  Type GetType() override;
  std::string GetTitle() override;
  GURL GetURL() override;
  bool Activate() override;
  bool Close() override;

  // EmbeddedWorkerDevToolsAgentHost override.
  bool Matches(const SharedWorkerInstance& other) override;

 private:
  ~SharedWorkerDevToolsAgentHost() override;
  scoped_ptr<SharedWorkerInstance> shared_worker_;

  DISALLOW_COPY_AND_ASSIGN(SharedWorkerDevToolsAgentHost);
};

}  // namespace content

#endif  // CONTENT_BROWSER_DEVTOOLS_SHARED_WORKER_DEVTOOLS_AGENT_HOST_H_

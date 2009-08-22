// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/file_util.h"
#include "base/path_service.h"
#include "base/platform_thread.h"
#include "base/string_util.h"
#include "base/test_file_util.h"
#include "chrome/app/chrome_dll_resource.h"
#include "chrome/browser/net/url_request_mock_http_job.h"
#include "chrome/browser/download/save_package.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/test/automation/browser_proxy.h"
#include "chrome/test/automation/tab_proxy.h"
#include "chrome/test/ui/ui_test.h"
#include "net/url_request/url_request_unittest.h"

const char* const kTestDir = "save_page";

const char* const kAppendedExtension =
#if defined(OS_WIN)
    ".htm";
#else
    ".html";
#endif

class SavePageTest : public UITest {
 protected:
  SavePageTest() : UITest() {}

  void CheckFile(const FilePath& generated_file,
                 const FilePath& expected_result_file) {
    FilePath expected_result_filepath = UITest::GetTestFilePath(
        UTF8ToWide(kTestDir), expected_result_file.ToWStringHack());
    ASSERT_TRUE(file_util::PathExists(expected_result_filepath));
    WaitForGeneratedFileAndCheck(generated_file,
                                 expected_result_filepath,
                                 false,  // Don't care whether they are equal.
                                 false,
                                 false);  // Don't delete file
  }

  virtual void SetUp() {
    UITest::SetUp();
    EXPECT_TRUE(file_util::CreateNewTempDirectory(FILE_PATH_LITERAL(""),
                                                  &save_dir_));

    download_dir_ = FilePath::FromWStringHack(GetDownloadDirectory());
  }

  virtual void TearDown() {
    UITest::TearDown();
    file_util::DieFileDie(save_dir_, true);
  }

  FilePath save_dir_;
  FilePath download_dir_;
};

// This tests that a webpage with the title "test.exe" is saved as
// "test.exe.htm".
// We probably don't care to handle this on Linux or Mac.
#if defined(OS_WIN)
TEST_F(SavePageTest, CleanFilenameFromPageTitle) {
  std::string file_name = "c.htm";
  FilePath full_file_name =
      download_dir_.AppendASCII(std::string("test.exe") + kAppendedExtension);
  FilePath dir = download_dir_.AppendASCII("test.exe_files");

  GURL url = URLRequestMockHTTPJob::GetMockUrl(
    UTF8ToWide(std::string(kTestDir) + "/" + file_name));
  scoped_refptr<TabProxy> tab(GetActiveTab());
  ASSERT_TRUE(tab.get());
  ASSERT_TRUE(tab->NavigateToURL(url));
  WaitUntilTabCount(1);

  scoped_refptr<BrowserProxy> browser(automation()->GetBrowserWindow(0));
  automation()->SavePackageShouldPromptUser(false);
  EXPECT_TRUE(browser->RunCommandAsync(IDC_SAVE_PAGE));
  EXPECT_TRUE(WaitForDownloadShelfVisible(browser.get()));
  automation()->SavePackageShouldPromptUser(true);

  CheckFile(full_file_name, FilePath::FromWStringHack(UTF8ToWide(file_name)));
  EXPECT_TRUE(file_util::DieFileDie(full_file_name, false));
  EXPECT_TRUE(file_util::DieFileDie(dir, true));
}
#endif

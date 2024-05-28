/// @file fetch.h
/// @brief This file contains definitions related to grabbing and parsing HTML
/// pages from wikipedia.
///
/// @author Ben Potter
/// @date 20 May 2024

#pragma once

#include <stdbool.h>

#include "array.h"


/// @struct page_html
/// @brief Stores the raw HTML data from fetching the wikipedia page.
typedef struct page_html {

    /// @brief Size of the page in bytes.
    size_t bytes;
    
    /// @brief HTML page as a string.
    char *html;

    /// @brief The title of the page.
    char title[256];

    /// @brief The number of links in the links member.
    size_t size_links;

    /// @brief Links to other pages.
    char **links;

} page_html;


/// @brief Initializes the page HTML structure.
///
/// @param html A pointer to the output page HTML structure.
/// @return True on success, otherwise false.
bool page_html_create(page_html *html);


/// @brief Fetches HTML via HTTP and cURL.
///
/// @warning This function allocates memory and attaches it to the page_html
/// structure. The user must ensure that this memory is freed using 
/// page_html_destroy.
///
/// @param html A pointer to the output page HTML structure.
/// @param url The HTTP URL target.
/// @return True on success, otherwise false.
bool page_html_fetch(page_html *html, const char *url);


/// @brief Scans the HTML tree for the tag holding the wikipedia page title.
///
/// @param html A reference to the HTML source where the title lives.
/// @return True on success, otherwise false.
bool page_html_extract_title(page_html *html);


/// @brief Scans the HTML tree for links to other wikipedia pages.
///
/// @param html A reference to the HTML source where the links live.
/// @return True on success, otherwise false.
bool page_html_extract_links(page_html *html);


/// @brief Destroys a page_html structure and deallocates its members.
///
/// @note It also deallocates the page_html structure itself.
///
/// @param html The page_html response to destroy.
/// @return True on success, otherwise false.
bool page_html_destroy(page_html *html);


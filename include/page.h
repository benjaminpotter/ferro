/// @file page.h
/// @brief This file contains definitions related to wikipedia pages.
///
/// @author Ben Potter
/// @date 20 May 2024

#pragma once

#include <stdbool.h>

#define MAX_PAGE_TITLE_SIZE 256


/// @enum PAGE_STATUS
/// @brief Used to track the page during its lifecycle.
typedef enum PAGE_STATUS { INVALID, LOADED } PAGE_STATUS;


/// @struct page
/// @brief Stores information about a single wikipedia page.
///
/// Pages are initialized in two steps. The first time a page is created, its
/// HTML source is fetched and the interesting data is extracted. This includes
/// its title and links to other wikipedia pages found in the HTML. At first,
/// these links are not traversed. Only when this page receives the focus, do we
/// iteratively create page structures for the grandchildren.
typedef struct page {
    
    /// @brief Title of the wikipedia page.
    char title[MAX_PAGE_TITLE_SIZE];

    /// @brief Used to load the page. Set during page initialization.
    char *url;

    /// @brief The current page status.
    PAGE_STATUS status;

    /// @brief Number of links away from this page.
    size_t size_links;

    /// @brief References to each linked page.
    struct page** links;

} page;


/// @brief Creates a page structure by injecting the configuration. This is the
/// first step in a page lifecycle.
///
/// @param page Pointer to hold the page.
/// @param url A URL string used to load page data.
/// @return True on success, otherwise false.
bool page_create(page* page, const char *url);


/// @brief Loads the page via HTML request using the config provided. Parses the
/// title and URLs to neighbouring pages.
///
/// @note Loading the page also requires caching the links to child pages. This 
/// is done to avoid sending multiple HTTP requests for the same page, or
/// requiring a local cache of the first HTTP request response. These cached
/// links are stored in the page_config structure on each page structure.
///
/// @warning This function allocates memory for each child page. The user must
/// ensure that page_destroy is called at the end of the page's life.
/// @see page_destroy
///
/// @param p The page to load.
/// @return True on success, otherwise false.
bool page_load(page *p);


/// @brief Given a parent page, load each of the children. Calls page_load for 
/// each child on a given page.
///
/// @param p The parent page whose children we want to load.
/// @return True on success, otherwise false.
bool page_load_children(page *p);


// @brief Destroys a page and deallocates it's children.
//
// @param p Pointer to the page to be destroyed.
// @return True on success, otherwise false.
bool page_destroy(page *p);


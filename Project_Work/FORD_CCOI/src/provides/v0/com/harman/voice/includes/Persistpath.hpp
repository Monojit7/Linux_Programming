#ifndef PERSISTPATH_HPP
#define PERSISTPATH_HPP

// CCOI directories
#define CCOI_ROOT                   "/mnt/data/ccoi"
#define DEFAULT_CCOI_ROOT           "/mnt/data/ccoi_default"
#define CCOI_ROOT_PERSISTPATH       "/mnt/data/ccoi/PersistPath"

// PTE
#define DEFAULT_PTE_FILE            DEFAULT_CCOI_ROOT     "/PTE"
#define DEFAULT_PTEHDR_BIN          DEFAULT_CCOI_ROOT     "/PTEHdr"
#define PTEHDR_BIN                  CCOI_ROOT_PERSISTPATH "/PTEHdr"
#define PTEHDRCOPY_BIN              CCOI_ROOT_PERSISTPATH "/PTEHdrCopy"
#define PTE_GZ                      CCOI_ROOT_PERSISTPATH "/PTE.gz"
#define PTE_FILE                    CCOI_ROOT_PERSISTPATH "/PTE"

// UFM
#define DEFAULT_UFM_FILE            DEFAULT_CCOI_ROOT     "/UFM"
#define DEFAULT_UFMHDR_BIN          DEFAULT_CCOI_ROOT     "/UFMHdr"
#define UFMHDR_BIN                  CCOI_ROOT_PERSISTPATH "/UFMHdr"
#define UFMHDRCOPY_BIN              CCOI_ROOT_PERSISTPATH "/UFMHdrCopy"
#define UFM_GZ                      CCOI_ROOT_PERSISTPATH "/UFM.gz"
#define UFM_FILE                    CCOI_ROOT_PERSISTPATH "/UFM"

// SYNC
#define DEFAULT_SYNC_FILE           DEFAULT_CCOI_ROOT     "/Sync"
#define SYNC_FILE                   CCOI_ROOT_PERSISTPATH "/Sync"

// settings
#define ENTITIES_DATA_PATH          CCOI_ROOT_PERSISTPATH "/Entities"

// UserPrompt
#define DEFPROMPTTOHMI              DEFAULT_CCOI_ROOT     "/DefPromptToHMI"
#define USER_PROMPT_RESPONSE_FILE   CCOI_ROOT_PERSISTPATH "/userPromptResponse"

// diagonistic file
#define DIAGNOSTIC_FILE             CCOI_ROOT_PERSISTPATH "/CcoiDiagnostics"

#endif  // PERSISTPATH_HPP

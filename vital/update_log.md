### Vital update log 

This log starts from 1.54.4

- Update 1.54.04 -> Smart pointer update - Added vtl::unique_ptr and its helper classes.
- Update 1.55.00 -> The entire pragma warning preprocessor macros have been updated to be specific to only MSVC.
- Update 1.55.01 -> All .cpp extension files have been moved to the detail folder.
- Update 1.56.00 -> locale.h development started. NOTE THAT: All 1.56.xx derivative updates are for locale.h
- Update 1.56.01 -> Work has started on win_cconv.h.
- Update 1.56.02 -> Work on win_cconv.h has been completed but some fixes need to be made.
- Update 1.56.03 -> Work has started on locale_encoding.h and locale_encoding.cpp.
- Update 1.56.04 -> Work has started on win_codepages.h.
- Update 1.56.05 -> Work on win_codepages.h has been completed.
- Update 1.56.06 -> Work on locale_encoding.h and locale_encoding.cpp has been completed.
- Update 1.56.07 -> Fixed some of the areas that needed fixing in win_cconv.h. It still needs fixes.
- Update 1.56.08 -> The files required for locale.h were moved to the locale folder within the detail folder.
- Update 1.56.09 -> locale.h has been updated to include only files and the contents have been moved to the encoding.h file in the locale folder in the detail folder.
- Update 1.56.10 -> Work has started on utf.h.
- Update 1.56.11 -> Work on utf.h has been completed.
- Update 1.56.12 -> Work has started on generic_codecvt.h.
- Update 1.56.13 -> generic_codecvt.h renamed codecvt.h. It now includes not only generic_codecvt but also utf_codecvt.
- Update 1.56.14 -> Work on codecvt.h has been completed.
- Update 1.56.15 -> encoding.h received an encoding utf update.
- Update 1.56.16 -> Remaining bugs in win_cconv.h have also been resolved. Only a few vtl alogrithms are missing. Instead of them, std is used for now.
- Update 1.56.17 -> Work has started on codepage.cpp.
- Update 1.56.18 -> locale.h DELETED!
- Update 1.57.00 -> Made some renaming in auto_ptr.h. There are no differences in usage.
- Update 1.57.01 -> Added minimal_smart_pointers.h file, which includes all minimal pointers.
- Update 1.57.02 -> Added operator= overload to minimal_unique_ptr and the explicit tag was removed from the constructor.
- Update 1.58.00 -> vector_iterator operator!= overload bug fixed.
- Update 1.58.01 -> Added new overload of seperate_string function.
- Update 1.58.02 -> conjuction struct reimplamented.
- Update 1.58.03 -> type_traits.h bugfixes.
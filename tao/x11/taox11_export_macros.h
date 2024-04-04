/**
 * @file    taox11_export_macros.h
 * @author  Unknown
 *
 * @brief   Export macros for building shared libraries
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_EXPORT_MACROS_H
#define TAOX11_EXPORT_MACROS_H

#define TAOX11_Proper_Export_Flag __attribute__ ((visibility("default")))
#define TAOX11_Proper_Import_Flag __attribute__ ((visibility("default")))

#define TAOX11_EXPORT_SINGLETON_DECLARATION(T) __extension__ extern template class TAOX11_Proper_Export_Flag T
#define TAOX11_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)  __extension__ extern template class TAOX11_Proper_Export_Flag SINGLETON_TYPE <CLASS, LOCK>;

#define TAOX11_IMPORT_SINGLETON_DECLARATION(T) __extension__ extern template class T
#define TAOX11_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) __extension__ extern template class SINGLETON_TYPE<CLASS, LOCK>;

#endif /* TAOX11_EXPORT_MACROS_H */

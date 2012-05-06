
/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/
/*
 *  Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_ListRefPtr_h
#define WTF_ListRefPtr_h

#include <wtf/RefPtr.h>

namespace ATF {

    // Specialized version of RefPtr desgined for use in singly-linked lists. 
    // Derefs the list iteratively to avoid recursive derefing that can overflow the stack.
    template <typename T> class ListRefPtr : public RefPtr<T> {
    public:
        ListRefPtr() : RefPtr<T>() {}
        ListRefPtr(T* ptr) : RefPtr<T>(ptr) {}
        ListRefPtr(const RefPtr<T>& o) : RefPtr<T>(o) {}
        // see comment in PassRefPtr.h for why this takes const reference
        template <typename U> ListRefPtr(const PassRefPtr<U>& o) : RefPtr<T>(o) {}
        
        ~ListRefPtr();
        
        ListRefPtr& operator=(T* optr) { RefPtr<T>::operator=(optr); return *this; }
        ListRefPtr& operator=(const RefPtr<T>& o) { RefPtr<T>::operator=(o); return *this; }
        ListRefPtr& operator=(const PassRefPtr<T>& o) { RefPtr<T>::operator=(o); return *this; }
        template <typename U> ListRefPtr& operator=(const RefPtr<U>& o) { RefPtr<T>::operator=(o); return *this; }
        template <typename U> ListRefPtr& operator=(const PassRefPtr<U>& o) { RefPtr<T>::operator=(o); return *this; }
    };

    // Remove inline for winscw compiler to prevent the compiler agressively resolving
    // T::ref() in RefPtr<T>'s copy constructor.  The bug is reported at:
    // https://xdabug001.ext.nokia.com/bugzilla/show_bug.cgi?id=9812.
    template <typename T> 
#if !COMPILER(WINSCW)
    inline
#endif
    ListRefPtr<T>::~ListRefPtr()
    {
        RefPtr<T> reaper = this->release();
        while (reaper && reaper->hasOneRef())
            reaper = reaper->releaseNext(); // implicitly protects reaper->next, then derefs reaper
    }

    template <typename T> inline T* getPtr(const ListRefPtr<T>& p)
    {
        return p.get();
    }

} // namespace ATF

using ATF::ListRefPtr;

#endif // WTF_ListRefPtr_h
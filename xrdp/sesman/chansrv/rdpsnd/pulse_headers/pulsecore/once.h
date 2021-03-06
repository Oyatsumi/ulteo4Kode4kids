#ifndef foopulseoncehfoo
#define foopulseoncehfoo

/* $Id: once.h 1971 2007-10-28 19:13:50Z lennart $ */

/***
  This file is part of PulseAudio.

  Copyright 2006 Lennart Poettering

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with PulseAudio; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#include <pulsecore/mutex.h>
#include <pulsecore/atomic.h>

typedef struct pa_once {
    pa_atomic_ptr_t mutex;
    pa_atomic_t ref, done;
} pa_once;

#define PA_ONCE_INIT                                                    \
    {                                                                   \
        .mutex = PA_ATOMIC_PTR_INIT(NULL),                              \
        .ref = PA_ATOMIC_INIT(0),                                       \
        .done = PA_ATOMIC_INIT(0)                                       \
    }

/* Not to be called directly, use the macros defined below instead */
int pa_once_begin(pa_once *o);
void pa_once_end(pa_once *o);

#define PA_ONCE_BEGIN                                                   \
    do {                                                                \
        static pa_once _once = PA_ONCE_INIT;                            \
        if (pa_once_begin(&_once)) {{

#define PA_ONCE_END                                                     \
            }                                                           \
            pa_once_end(&_once);                                        \
        }                                                               \
    } while(0)

/*

  Usage of these macros is like this:

  void foo() {

      PA_ONCE_BEGIN {

          ... stuff to be called just once ...

      } PA_ONCE_END;
  }

*/

/* Same API but calls a function */
typedef void (*pa_once_func_t) (void);
void pa_run_once(pa_once *o, pa_once_func_t f);

#endif

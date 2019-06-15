/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * mm/mm_heap/mm_size2ndx.c
 *
 *   Copyright (C) 2007, 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_size2ndx
 *
 * Description:
 *    Convert the size to a nodelist index.
 *
 ****************************************************************************/

int mm_size2ndx(size_t size)
{
	int ndx = 0;
	size_t o_size = size;
	size_t mask = MM_SHIFT_MASK;

	if ((size >> MM_MAX_SHIFT) > 0) {
		return MM_NNODES - 1;
	}

	size >>= MM_SHIFT_FOR_NDX;
	while (size > 1) {
		ndx++;
		size >>= 1;
	}
	
	/* If free nodes are sorted in a descending order,
	 * it would be better to use ]32, 64] instead of [32, 64[.
	 */
	if (size > 0) {
		mask <<= ndx;
		if ((o_size & (~mask)) > 0) {
			return ++ndx;
		} else {
			return ndx;
		}
	} else {
		return ndx;
	}
}

#ifdef CONFIG_MM_REALTIME_SUPPORT
/****************************************************************************
 * Name: mm_size2ndx_realtime
 *
 * Description:
 *    Convert the size to a nodelist index when realtime support is enabled.
 *
 ****************************************************************************/

int mm_size2ndx_realtime(FAR struct mm_heap_s *heap, size_t size)
{
	int ndx = 0;
	size_t o_size = size;
	size_t mask = MM_SHIFT_MASK;

	if ((size >> MM_MAX_SHIFT) > 0) {
		return MM_NNODES - 1;
	}

	size >>= MM_MIN_SHIFT;
	if (size > MM_REALTIME_SUPPORT_NUMOF_SIZES + 1) {
		size >>= 1;
		while (size > 1) {
			ndx++;
			size >>= 1;
		}

		/* If free nodes are sorted in a descending order,
		 * it would be better to use ]32, 64] instead of [32, 64[.
		 */
		mask <<= ndx;
		if ((o_size & (~mask)) > 0) {
			++ndx;
		}
		return ndx + heap->mm_ndx_offset;
	} else if (size < 3) {
		return 0;
	}
	return size - 2;
}
#endif

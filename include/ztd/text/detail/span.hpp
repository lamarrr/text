// =============================================================================
//
// ztd.text
// Copyright © 2021 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.text licenses may use this file in
// accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License") for non-commercial use; you may not use this
// file except in compliance with the License. You may obtain a copy of the 
// License at
//
//		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// =============================================================================

#pragma once

#ifndef ZTD_TEXT_DETAIL_SPAN_HPP
#define ZTD_TEXT_DETAIL_SPAN_HPP

#include <ztd/text/version.hpp>

#include <ztd/text/detail/span.implementation.hpp>

#if !defined(span_HAVE_STD_SPAN) || span_HAVE_STD_SPAN == 0

namespace std {

	inline constexpr decltype(::nonstd::dynamic_extent) dynamic_extent = ::nonstd::dynamic_extent;

	using ::nonstd::span_lite::as_bytes;
	using ::nonstd::span_lite::as_writable_bytes;
	using ::nonstd::span_lite::span;

} // namespace std

#endif // Have Standard Span

#endif // ZTD_TEXT_DETAIL_SPAN_HPP
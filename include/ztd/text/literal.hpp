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

#ifndef ZTD_TEXT_LITERAL_HPP
#define ZTD_TEXT_LITERAL_HPP

#include <ztd/text/version.hpp>

#include <ztd/text/unicode_code_point.hpp>

#include <ztd/text/state.hpp>
#include <ztd/text/code_point.hpp>
#include <ztd/text/code_unit.hpp>

#include <ztd/text/detail/ebco.hpp>
#include <ztd/text/detail/encoding_name.hpp>
#include <ztd/text/detail/forwarding_handler.hpp>

namespace ztd { namespace text {
	ZTD_TEXT_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __detail {
		inline constexpr __encoding_id __literal_id = __to_encoding_id(ZTD_TEXT_COMPILE_TIME_ENCODING_NAME_GET_I_());
		using __literal                             = decltype(__select_encoding<char, __literal_id>());
	} // namespace __detail

	//////
	/// @brief The encoding of wide string literals (e.g. @c "L\"👍\"") at compile-time.
	///
	//////
	class literal : private __detail::__ebco<__detail::__literal> {
	private:
		using __underlying_t = __detail::__literal;
		using __base_t       = __detail::__ebco<__underlying_t>;

	public:
		//////
		/// @brief The individual units that result from an encode operation or are used as input to a decode
		/// operation.
		//////
		using code_unit = encoding_code_unit_t<__underlying_t>;
		//////
		/// @brief The individual units that result from a decode operation or as used as input to an encode
		/// operation. For most encodings, this is going to be a Unicode Code Point or a Unicode Scalar Value.
		//////
		using code_point = encoding_code_point_t<__underlying_t>;
		//////
		/// @brief The state that can be used between calls to encode_one.
		///
		//////
		using encode_state = encoding_encode_state_t<__underlying_t>;
		//////
		/// @brief The state that can be used between calls to decode_one.
		///
		//////
		using decode_state = encoding_decode_state_t<__underlying_t>;
		//////
		/// @brief Whether or not the decode operation can process all forms of input into code point values.
		///
		/// @remarks The decode step is always injective because every encoding used for literals in C++ needs to be
		/// capable of being represented by UCNs.
		//////
		using is_decode_injective = ::std::integral_constant<bool, is_decode_injective_v<__underlying_t>>;
		//////
		/// @brief Whether or not the encode operation can process all forms of input into code unit values.
		///
		/// @remarks This is absolutely not guaranteed to be the case, and as such we must check the provided encoding
		/// name for wide to be sure.
		//////
		using is_encode_injective = ::std::integral_constant<bool, is_encode_injective_v<__underlying_t>>;

		//////
		/// @brief The maximum number of code points a single complete operation of decoding can produce.
		///
		//////
		inline static constexpr ::std::size_t max_code_points = 8;
		//////
		/// @brief The maximum code units a single complete operation of encoding can produce.
		///
		//////
		inline static constexpr ::std::size_t max_code_units = 16;

		using __base_t::__base_t;

		//////
		/// @brief Decodes a single complete unit of information as code points and produces a result with the
		/// input and output ranges moved past what was successfully read and written; or, produces an error and
		/// returns the input and output ranges untouched.
		///
		/// @param[in] __input The input view to read code uunits from.
		/// @param[in] __output The output view to write code points into.
		/// @param[in] __error_handler The error handler to invoke if encoding fails.
		/// @param[in, out] __state The necessary state information. For this encoding, the state is empty and means
		/// very little.
		///
		/// @returns A ztd::text::decode_result object that contains the reconstructed input range,
		/// reconstructed output range, error handler, and a reference to the passed-in state.
		///
		/// @remarks To the best ability of the implementation, the iterators will be returned untouched (e.g.,
		/// the input models at least a view and a forward_range). If it is not possible, returned ranges may be
		/// incremented even if an error occurs due to the semantics of any view that models an input_range.
		//////
		template <typename _Input, typename _Output, typename _ErrorHandler>
		constexpr auto decode_one(
			_Input&& __input, _Output&& __output, _ErrorHandler&& __error_handler, decode_state& __state) const {
#if ZTD_TEXT_IS_OFF(ZTD_TEXT_COMPILE_TIME_ENCODING_NAME_I_) \
     && ZTD_TEXT_IS_OFF(ZTD_TEXT_YES_PLEASE_FUCK_MY_LITERALS_UP_I_MEAN_IT_I_)
			// Cry bitter tears, I guess?
			static_assert(__detail::__always_false_v<_Input>,
				"[[ PLEASE. READ. ]] Your compiler does not implement any known way of getting the string literal "
				"encoding from the machine at compile-time and you are trying to use the literal encoding "
				"type, somewhere. If this is the Microsoft Visual C/C++ Compiler (MSVC), then go upvote this issue "
				"here (https://developercommunity.visualstudio.com/content/idea/1160821/"
				"-compiler-feature-macro-for-narrow-literal-foo-enc.html) and leave a (strong, but very nicely "
				"spoken) request to implement the functionality. If you are part of the C++ Standards Committee, "
				"please make sure p1885 (https://wg21.link/p1885) gets passed so we don't need to keep doing this "
				"nonsense. If you are from Clang, please see my comment here "
				"(https://reviews.llvm.org/D88741#2352203) and kindly encourage them to implement this. If you are "
				"on some other compiler,\n\n"

				"God's Speed.\n\n"

				"This could mangle all of your compile-time literal string conversions you are performing. To "
				"prevent that from happening, this error is being printed out and to go ahead with the explicit "
				"understanding that your life may indeed suck after doing this.\n\n"

				"Please add ZTD_TEXT_YES_PLEASE_FUCK_MY_LITERALS_UP_I_MEAN_IT to your command line to ignore "
				"this "
				"error and we will get right on doing exactly that for you.");
#endif
			__detail::__forwarding_handler<const literal, __detail::__remove_cvref_t<_ErrorHandler>>
				__underlying_handler(*this, __error_handler);
			return this->__base_t::get_value().decode_one(
				::std::forward<_Input>(__input), ::std::forward<_Output>(__output), __underlying_handler, __state);
		}

		//////
		/// @brief Encodes a single complete unit of information as code units and produces a result with the
		/// input and output ranges moved past what was successfully read and written; or, produces an error and
		/// returns the input and output ranges untouched.
		///
		/// @param[in] __input The input view to read code points from.
		/// @param[in] __output The output view to write code units into.
		/// @param[in] __error_handler The error handler to invoke if encoding fails.
		/// @param[in, out] __state The necessary state information. For this encoding, the state is empty and means
		/// very little.
		///
		/// @returns A ztd::text::encode_result object that contains the reconstructed input range,
		/// reconstructed output range, error handler, and a reference to the passed-in state.
		///
		/// @remarks To the best ability of the implementation, the iterators will be returned untouched (e.g.,
		/// the input models at least a view and a forward_range). If it is not possible, returned ranges may be
		/// incremented even if an error occurs due to the semantics of any view that models an input_range.
		//////
		template <typename _Input, typename _Output, typename _ErrorHandler>
		constexpr auto encode_one(
			_Input&& __input, _Output&& __output, _ErrorHandler&& __error_handler, encode_state& __state) const {
#if ZTD_TEXT_IS_OFF(ZTD_TEXT_COMPILE_TIME_ENCODING_NAME_I_) \
     && ZTD_TEXT_IS_OFF(ZTD_TEXT_YES_PLEASE_FUCK_MY_LITERALS_UP_I_MEAN_IT_I_)
			// Cry bitter tears, I guess?
			static_assert(__detail::__always_false_v<_Input>,
				"[[ PLEASE. READ. ]] Your compiler does not implement any known way of getting the string literal "
				"encoding from the machine at compile-time and you are trying to use the literal encoding "
				"type, somewhere. If this is the Microsoft Visual C/C++ Compiler (MSVC), then go upvote this issue "
				"here (https://developercommunity.visualstudio.com/content/idea/1160821/"
				"-compiler-feature-macro-for-narrow-literal-foo-enc.html) and leave a (strong, but very nicely "
				"spoken) request to implement the functionality. If you are part of the C++ Standards Committee, "
				"please make sure p1885 (https://wg21.link/p1885) gets passed so we don't need to keep doing this "
				"nonsense. If you are from Clang, please see my comment here "
				"(https://reviews.llvm.org/D88741#2352203) and kindly encourage them to implement this. If you are "
				"on some other compiler,\n\n"

				"God's Speed.\n\n"

				"This could mangle all of your compile-time literal string conversions you are performing. To "
				"prevent that from happening, this error is being printed out and to go ahead with the explicit "
				"understanding that your life may indeed suck after doing this.\n\n"

				"Please add ZTD_TEXT_YES_PLEASE_FUCK_MY_LITERALS_UP_I_MEAN_IT to your command line to ignore "
				"this "
				"error and we will get right on doing exactly that for you.");
#endif
			__detail::__forwarding_handler<const literal, __detail::__remove_cvref_t<_ErrorHandler>>
				__underlying_handler(*this, __error_handler);
			return this->__base_t::get_value().encode_one(
				::std::forward<_Input>(__input), ::std::forward<_Output>(__output), __underlying_handler, __state);
		}
	};

	ZTD_TEXT_INLINE_ABI_NAMESPACE_CLOSE_I_
}} // namespace ztd::text

#endif // ZTD_TEXT_LITERAL_HPP
#pragma once
#ifndef INSTALLER_UTF16_H
#define INSTALLER_UTF16_H
#include <locale>

class UTF16Facet : public std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type>
{
	typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type> MyType;
	typedef MyType::state_type          state_type;
	typedef MyType::result              result;


	/* This function deals with converting data from the input stream into the internal stream.*/
	/*
	* from, from_end:  Points to the beginning and end of the input that we are converting 'from'.
	* to,   to_limit:  Points to where we are writing the conversion 'to'
	* from_next:       When the function exits this should have been updated to point at the next location
	*                  to read from. (ie the first unconverted input character)
	* to_next:         When the function exits this should have been updated to point at the next location
	*                  to write to.
	*
	* status:          This indicates the status of the conversion.
	*                  possible values are:
	*                  error:      An error occurred the bad file bit will be set.
	*                  ok:         Everything went to plan
	*                  partial:    Not enough input data was supplied to complete any conversion.
	*                  nonconv:    no conversion was done.
	*/
	virtual result  do_in(state_type &s,
		const char  *from, const char *from_end, const char* &from_next,
		wchar_t     *to, wchar_t    *to_limit, wchar_t*    &to_next) const
	{
		// Loop over both the input and output array/
		for (; (from < from_end) && (to < to_limit); from += 2, ++to)
		{
			/*Input the Data*/
			/* As the input 16 bits may not fill the wchar_t object
			* Initialise it so that zero out all its bit's. This
			* is important on systems with 32bit wchar_t objects.
			*/
			(*to) = L'\0';

			/* Next read the data from the input stream into
			* wchar_t object. Remember that we need to copy
			* into the bottom 16 bits no matter what size the
			* the wchar_t object is.
			*/
			reinterpret_cast<char*>(to)[0] = from[0];
			reinterpret_cast<char*>(to)[1] = from[1];
		}
		from_next = from;
		to_next = to;

		return((from > from_end) ? partial : ok);
	}



	/* This function deals with converting data from the internal stream to a C/C++ file stream.*/
	/*
	* from, from_end:  Points to the beginning and end of the input that we are converting 'from'.
	* to,   to_limit:  Points to where we are writing the conversion 'to'
	* from_next:       When the function exits this should have been updated to point at the next location
	*                  to read from. (ie the first unconverted input character)
	* to_next:         When the function exits this should have been updated to point at the next location
	*                  to write to.
	*
	* status:          This indicates the status of the conversion.
	*                  possible values are:
	*                  error:      An error occurred the bad file bit will be set.
	*                  ok:         Everything went to plan
	*                  partial:    Not enough input data was supplied to complete any conversion.
	*                  nonconv:    no conversion was done.
	*/
	virtual result do_out(state_type &state,
		const wchar_t *from, const wchar_t *from_end, const wchar_t* &from_next,
		char          *to, char          *to_limit, char*          &to_next) const
	{
		for (; (from < from_end) && (to < to_limit); ++from, to += 2)
		{
			/* Output the Data */
			/* NB I am assuming the characters are encoded as UTF-16.
			* This means they are 16 bits inside a wchar_t object.
			* As the size of wchar_t varies between platforms I need
			* to take this into consideration and only take the bottom
			* 16 bits of each wchar_t object.
			*/
			to[0] = reinterpret_cast<const char*>(from)[0];
			to[1] = reinterpret_cast<const char*>(from)[1];

		}
		from_next = from;
		to_next = to;

		return((to > to_limit) ? partial : ok);
	}
};
#endif
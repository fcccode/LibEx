
#include "baserelocs.h"

namespace pelite
{
	
BaseRelocIterator::BaseRelocIterator( const PeFile& bin ) :
	relocs( bin.OptionalHeader()->DataDirectory[ DataDir_BaseReloc ] ),
	outer_it(nullptr), outer_end(nullptr), inner_it(nullptr), inner_end(nullptr)
{
	if ( relocs.VirtualAddress )
	{
		outer_it = bin.RvaToPtr<ImageBaseRelocation*>( relocs.VirtualAddress );
		outer_end = (ImageBaseRelocation*)( (char*)outer_it + relocs.Size );
		inner_it = (ImageBaseRelocBlock*)( outer_it + 1 );
		inner_end = (ImageBaseRelocBlock*)( (char*)outer_it + outer_it->SizeOfBlock );
	}
}
void BaseRelocIterator::Next()
{
	// Check if end of inner
	if ( inner_it>=inner_end )
	{
		outer_it = (ImageBaseRelocation*)( (char*)outer_it + outer_it->SizeOfBlock );
		inner_it = (ImageBaseRelocBlock*)( outer_it + 1 );
	}
	else
	{
		++inner_it;
	}
}


BaseRelocs::BaseRelocs( const PeFile& bin ) : mPeFile(bin)
{
	ImageDataDirectory& dir = bin.OptionalHeader()->DataDirectory[ DataDir_BaseReloc ];

	if ( !dir.VirtualAddress )
	{
		throw std::exception( "Invalid" );
	}

	//mRelocs = bin.RvaToPtr<ImageBaseRelocation*>( dir.VirtualAddress );
}
bool BaseRelocs::Exists( const PeFile& bin )
{
	return bin.OptionalHeader()->DataDirectory[ DataDir_BaseReloc ].VirtualAddress != 0;
}
unsigned BaseRelocs::PreferredLoadAddress() const
{
	return mPeFile.OptionalHeader()->ImageBase;
}
ImageDataDirectory& BaseRelocs::DataDir() const
{
	return mPeFile.OptionalHeader()->DataDirectory[DataDir_BaseReloc];
}
ImageBaseRelocation* BaseRelocs::Raw() const
{
	return mPeFile.RvaToPtr<ImageBaseRelocation*>( DataDir().VirtualAddress );
}
unsigned BaseRelocs::RawSize() const
{
	return DataDir().Size;
}

}

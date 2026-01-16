#pragma once

#include "vhrect.hpp"
#include "vhmemblk.hpp"
#include "svg.hpp"


void	DrawShadowBox	( const VHRect & rect );
void	DrawScale		( const VHRect & rect , int size);

verr	GenerateDocument( const VHMemBlock & memblk );

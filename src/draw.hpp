#pragma once

#include "vhrect.hpp"
#include "vhmemblk.hpp"
#include "misc.hpp"
#include "svg.hpp"



void	DrawShadowBox	( const VHRect & rect );

void	DrawScale		( const VHRect & rect , int size);
void	DrawTypes		( const VHRect & rect );

void	DrawSection		( const VHRect & rect , const std::vector<stret> & arr , int blksize, int ws );
void	DrawDensity		( const VHRect & rect , const std::vector<stret> & arr , int blksize, int ws );

verr	GenerateDocument( const VHMemBlock & memblk );

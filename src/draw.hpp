#pragma once

#include "vhrect.hpp"
#include "vhmemblk.hpp"
#include "misc.hpp"
#include "svg.hpp"


void	DrawCenterLine	( const VHRect & rect );

void	DrawShadowBox	( const VHRect & rect );

void	DrawScale		( const VHRect & rect , int size);

void	DrawSection		( const VHRect & rect , const std::vector<BlockProps> & arr , int blksize, int ws );
void	DrawDensity		( const VHRect & rect , const std::vector<BlockProps> & arr , int blksize, int ws );

void	DrawTypes		( const VHRect & rect );
void	DrawProps		( const VHRect & rect );


verr	GenerateDocument( const VHMemBlock & memblk );

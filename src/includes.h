/**
 *  Includes.h
 * 
 *  Header file that includes all public header files of the SmartTpl library
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Dependencies
 */
#include <string>
#include <iostream>

/**
 *  All public header files of the SmartTpl library
 */
#include "../include/data.h"
#include "../include/template.h"

/**
 *  Implementation-only header files
 */
#include "token.h"
#include "parser.h"
#include "tokenizer.h"
#include "syntaxtree.h"
#include "lemon.h"

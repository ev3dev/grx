Unit SH_Funcs;

{
 * Collection of useful things for working with GPC
 * needed for compiling the BGI2GRX demos
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author : Sven Hilscher
 * e-mail : sven@rufus.central.de
}

Interface


Const
  Pi    = 3.14159265358979323846;

Type
  WrkString = String[255];
  PChar = ^char;

Function KeyPressed: Boolean; AsmName 'kbhit';
Function ReadKey: Char; AsmName 'getch';
Function Random(Limit: Word): Word;

Implementation

Function Random(Limit: Word): Word;
  Function Rand: Word; C;
begin
  Random := Rand mod Limit;
end;


end.


SECTIONS
{
    . = 0x80018000;
    .text : {
		* (.start);
        * (.text);
    }
}

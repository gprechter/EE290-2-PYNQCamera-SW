SECTIONS
{
    . = 0x8000C000;
    .text : {
		* (.start);
        * (.text);
    }
}

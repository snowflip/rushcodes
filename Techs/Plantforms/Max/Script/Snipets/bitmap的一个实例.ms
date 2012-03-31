b=selectbitmap() -- open image file browser
bname = "bitmap_"+(getfilenamefile b.filename) -- build name from filename

-- get properties of bitmap
w=b.width 
h=b.height

format "----------\nfn load_% = (\n" bname -- start defining function
format "local %=bitmap % %\n" bname w h -- create bitmap in function
-- write out a function that unpacks an integer into a pixel color
format "fn unpack val = for p in val collect (r=p/256^2; g=p/256-r*256; b=mod p 256; color r g b)\n"

for r=0 to h-1 do -- for each row in the bitmap
-- have function write the column of pixels to the bitmap
( 
		format "setpixels % [0,%] (unpack #(" bname r
		
		pixels=getpixels b [0,r] w -- read in the column of pixels

		for c=1 to w do -- loop through each pixel
		( 
			p=pixels[c] -- get the pixel

			-- pack the pixel into an integer and write it out
			format "%" (((p.r as integer)*256+(p.g as integer))*256+(p.b as integer))

			if c != w then -- if not at end of data
				format ", " -- write a comma
			else
				format "))\n" -- else close out the line

		)
)

format "return %\n" bname -- function returns the bitmap
format ")\n----------\n" -- finish off function definition

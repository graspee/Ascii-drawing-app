#this is the version for a 16x16 grid of 16x15 pixel characters

require 'RMagick'

print "using <#{ARGV[0]}>"
png=Magick::ImageList.new ARGV[0]
image=png.to_blob {self.format="rgba"}
# size is 65536 (16 chars by 16 chars, each 8x8 pixels, rgba each pixel)



print "const unsigned char fontname[256][32] = {\n"

(0...256).each do |c|

	print "{"


	(0...16).each do |line|
	
		
#16384=16 chars times 16 pixels per char * 4 bytes per pixels * 16 rows per char
#64 is 16 pixels per row times 4 bytes
#16*16*4 is 16 chars 16 pixels 4 bytes
		startpos=(c.divmod(16)[0]*16384)	+ (c.divmod(16)[1]*64) + (line*16*16*4)
	
		binstr=""
		(0...16).each do |f|
			binstr=binstr+(if image[startpos+(f*4)]=="\xFF" then "1" else "0" end)
		end
	#print binstr
	#print "\n"
	print (binstr[0..7]).to_i(2).to_s
	print ","
	print (binstr[8..15]).to_i(2).to_s
	if line < 15 then print "," end
	end #foreach line
	print "}"
	if c<255 then print "," end
	print "\n"
end #foreach char

#each line is {a,b,c}, but no comma on last line

print "};\n"

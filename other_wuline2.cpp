void WuLine(float x1,float y1,float x2,float y2,BYTE r,BYTE g,BYTE b)
{
	float xd,yd,grad,xend,yend,xgap,b1,b2,yf;
	int ix1,ix2,iy1,iy2;
	DWORD c1,c2;

	xd = x2-x1;
	yd = y2-y1;

	if (fabs(xd)>=fabs(yd))
	{
		// Horizontal slope


		if (x1>x2)
		{
			float tmp=x1;
			x1=x2;
			x2=tmp;
			tmp=y1;
			y1=y2;
			y2=tmp;
			xd = x2-x1;
			yd = y2-y1;
		}

		yf = y1;
		grad = yd/xd;
		ix1 = ifloor(x1);
		ix2 = ifloor(x2);
		for (int x=ix1; x<=ix2; x++)
		{
			b2 = yf-float(ifloor(yf));
			b1 = 1.0f-b2;
			
			DWORD tmp=(ifloor(yf)*WIDTH)+x;
			c1=RGB(BYTE(b1*r),BYTE(b1*g),BYTE(b1*b));
			screen[tmp]=c1;
			c1=RGB(BYTE(b2*r),BYTE(b2*g),BYTE(b2*b));
			tmp += WIDTH;
			screen[tmp]=c1;
			
			yf += grad;
		}
	} else
	{
		if (y1>y2)
		{
			float tmp=x1;
			x1=x2;
			x2=tmp;
			tmp=y1;
			y1=y2;
			y2=tmp;
			xd = x2-x1;
			yd = y2-y1;
		}

		yf = x1;
		grad = xd/yd;
		iy1 = ifloor(y1);
		iy2 = ifloor(y2);
		for (int y=iy1; y<=iy2; y++)
		{
			b2 = yf-float(ifloor(yf));
			b1 = 1.0f-b2;
			
			DWORD tmp=(y*WIDTH)+ifloor(yf);
			c1=RGB(BYTE(b1*r),BYTE(b1*g),BYTE(b1*b));
			screen[tmp]=c1;
			c1=RGB(BYTE(b2*r),BYTE(b2*g),BYTE(b2*b));
			tmp++;
			screen[tmp]=c1;
			
			yf += grad;
		}			
	}
}
  

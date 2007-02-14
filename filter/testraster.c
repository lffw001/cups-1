/*
 * "$Id: testraster.c 6158 2006-12-17 01:44:21Z mike $"
 *
 *   Raster test program routines for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 1997-2005 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights for the CUPS Raster source
 *   files are outlined in the GNU Library General Public License, located
 *   in the "pstoraster" directory.  If this file is missing or damaged
 *   please contact Easy Software Products at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636 USA
 *
 *       Voice: (301) 373-9600
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 *
 *   This code and any derivative of it may be used and distributed
 *   freely under the terms of the GNU General Public License when
 *   used with GNU Ghostscript or its derivatives.  Use of the code
 *   (or any derivative of it) with software other than GNU
 *   GhostScript (or its derivatives) is governed by the CUPS license
 *   agreement.
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   main()          - Test the raster functions.
 *   print_changes() - Print differences in the page header.
 */

/*
 * Include necessary headers...
 */

#include "image-private.h"
#include <stdlib.h>
#include <cups/string.h>


/*
 * Test PS commands and header...
 */

static const char *test_code =
"<<"
"/MediaClass(Media Class)"
"/MediaColor((Media Color))"
"/MediaType(Media\\\\Type)"
"/OutputType<416263>"
"/AdvanceDistance 1000"
"/AdvanceMedia 1"
"/Collate false"
"/CutMedia 2"
"/Duplex true"
"/HWResolution[100 200]"
"/InsertSheet true"
"/Jog 3"
"/LeadingEdge 1"
"/ManualFeed true"
"/MediaPosition 8#777"
"/MediaWeight 16#fe01"
"/MirrorPrint true"
"/NegativePrint true"
"/NumCopies 1"
"/Orientation 1"
"/OutputFaceUp true"
"/PageSize[612 792.1]"
"/Separations true"
"/TraySwitch true"
"/Tumble true"
"/cupsMediaType 2"
"/cupsColorOrder 1"
"/cupsColorSpace 1"
"/cupsCompression 1"
"/cupsRowCount 1"
"/cupsRowFeed 1"
"/cupsRowStep 1"
"/cupsBorderlessScalingFactor 1.001"
"/cupsInteger0 1"
"/cupsInteger1 2"
"/cupsInteger2 3"
"/cupsInteger3 4"
"/cupsInteger4 5"
"/cupsInteger5 6"
"/cupsInteger6 7"
"/cupsInteger7 8"
"/cupsInteger8 9"
"/cupsInteger9 10"
"/cupsInteger10 11"
"/cupsInteger11 12"
"/cupsInteger12 13"
"/cupsInteger13 14"
"/cupsInteger14 15"
"/cupsInteger15 16"
"/cupsReal0 1.1"
"/cupsReal1 2.1"
"/cupsReal2 3.1"
"/cupsReal3 4.1"
"/cupsReal4 5.1"
"/cupsReal5 6.1"
"/cupsReal6 7.1"
"/cupsReal7 8.1"
"/cupsReal8 9.1"
"/cupsReal9 10.1"
"/cupsReal10 11.1"
"/cupsReal11 12.1"
"/cupsReal12 13.1"
"/cupsReal13 14.1"
"/cupsReal14 15.1"
"/cupsReal15 16.1"
"/cupsString0(1)"
"/cupsString1(2)"
"/cupsString2(3)"
"/cupsString3(4)"
"/cupsString4(5)"
"/cupsString5(6)"
"/cupsString6(7)"
"/cupsString7(8)"
"/cupsString8(9)"
"/cupsString9(10)"
"/cupsString10(11)"
"/cupsString11(12)"
"/cupsString12(13)"
"/cupsString13(14)"
"/cupsString14(15)"
"/cupsString15(16)"
"/cupsMarkerType(Marker Type)"
"/cupsRenderingIntent(Rendering Intent)"
"/cupsPageSizeName(Letter)"
"/cupsPreferredBitsPerColor 17"
">> setpagedevice";

static cups_page_header2_t test_header =
{
  "Media Class",			/* MediaClass */
  "(Media Color)",			/* MediaColor */
  "Media\\Type",			/* MediaType */
  "Abc",				/* OutputType */
  1000,					/* AdvanceDistance */
  CUPS_ADVANCE_FILE,			/* AdvanceMedia */
  CUPS_FALSE,				/* Collate */
  CUPS_CUT_JOB,				/* CutMedia */
  CUPS_TRUE,				/* Duplex */
  { 100, 200 },				/* HWResolution */
  { 0, 0, 0, 0 },			/* ImagingBoundingBox */
  CUPS_TRUE,				/* InsertSheet */
  CUPS_JOG_SET,				/* Jog */
  CUPS_EDGE_RIGHT,			/* LeadingEdge */
  { 0, 0 },				/* Margins */
  CUPS_TRUE,				/* ManualFeed */
  0777,					/* MediaPosition */
  0xfe01,				/* MediaWeight */
  CUPS_TRUE,				/* MirrorPrint */
  CUPS_TRUE,				/* NegativePrint */
  1,					/* NumCopies */
  CUPS_ORIENT_90,			/* Orientation */
  CUPS_TRUE,				/* OutputFaceUp */
  { 612, 792 },				/* PageSize */
  CUPS_TRUE,				/* Separations */
  CUPS_TRUE,				/* TraySwitch */
  CUPS_TRUE,				/* Tumble */
  0,					/* cupsWidth */
  0,					/* cupsHeight */
  2,					/* cupsMediaType */
  0,					/* cupsBitsPerColor */
  0,					/* cupsBitsPerPixel */
  0,					/* cupsBytesPerLine */
  CUPS_ORDER_BANDED,			/* cupsColorOrder */
  CUPS_CSPACE_RGB,			/* cupsColorSpace */
  1,					/* cupsCompression */
  1,					/* cupsRowCount */
  1,					/* cupsRowFeed */
  1,					/* cupsRowStep */
  0,					/* cupsNumColors */
  1.001,				/* cupsBorderlessScalingFactor */
  { 612.0, 792.1 },			/* cupsPageSize */
  { 0.0, 0.0, 0.0, 0.0 },		/* cupsImagingBBox */
  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
					/* cupsInteger[16] */
  { 1.1, 2.1, 3.1, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1, 10.1, 11.1, 12.1, 13.1,
    14.1, 15.1, 16.1 },			/* cupsReal[16] */
  { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
    "14", "15", "16" },			/* cupsString[16] */
  "Marker Type",			/* cupsMarkerType */
  "Rendering Intent",			/* cupsRenderingIntent */
  "Letter"				/* cupsPageSizeName */
};


/*
 * Local functions...
 */

static void	print_changes(cups_page_header2_t *header,
		              cups_page_header2_t *expected);


/*
 * 'main()' - Test the raster functions.
 */

int					/* O - Exit status */
main(void)
{
  int			page, x, y;	/* Looping vars */
  FILE			*fp;		/* Raster file */
  cups_raster_t		*r;		/* Raster stream */
  cups_page_header2_t	header,		/* Page header */
			expected;	/* Expected page header */
  unsigned char		data[2048];	/* Raster data */
  int			preferred_bits;	/* Preferred bits */
  int			errors;		/* Number of errors */


  errors = 0;

 /*
  * Test PS exec code...
  */

  fputs("_cupsRasterExecPS(\"setpagedevice\"): ", stdout);
  fflush(stdout);

  memset(&header, 0, sizeof(header));
  header.Collate = CUPS_TRUE;
  preferred_bits = 0;

  if (_cupsRasterExecPS(&header, &preferred_bits, test_code))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (preferred_bits != 17 || memcmp(&header, &test_header, sizeof(header)))
  {
    puts("FAIL (bad header)");

    if (preferred_bits != 17)
      printf("    cupsPreferredBitsPerColor %d, expected 17\n",
             preferred_bits);

    print_changes(&test_header, &header);
    errors ++;
  }
  else
    puts("PASS");
    
  fputs("_cupsRasterExecPS(\"roll\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        "792 612 0 0 0\n"
			"pop pop pop\n"
                	"<</PageSize[5 -2 roll]/ImagingBBox null>>"
			"setpagedevice\n"))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (header.PageSize[0] != 792 || header.PageSize[1] != 612)
  {
    printf("FAIL (PageSize [%d %d], expected [792 612])\n", header.PageSize[0],
           header.PageSize[1]);
    errors ++;
  }
  else
    puts("PASS");

  fputs("_cupsRasterExecPS(\"dup index\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        "true false dup\n"
			"<</Collate 4 index"
			"/Duplex 5 index"
			"/Tumble 6 index>>setpagedevice\n"
			"pop pop pop"))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else
  {
    if (!header.Collate)
    {
      printf("FAIL (Collate false, expected true)\n");
      errors ++;
    }

    if (header.Duplex)
    {
      printf("FAIL (Duplex true, expected false)\n");
      errors ++;
    }

    if (header.Tumble)
    {
      printf("FAIL (Tumble true, expected false)\n");
      errors ++;
    }

    if(header.Collate && !header.Duplex && !header.Tumble)
      puts("PASS");
  }

#if 0
  fputs("_cupsRasterExecPS(\"\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        ""))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (header.)
  {
    printf("FAIL ()\n");
    errors ++;
  }
  else
    puts("PASS");

  fputs("_cupsRasterExecPS(\"\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        ""))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (header.)
  {
    printf("FAIL ()\n");
    errors ++;
  }
  else
    puts("PASS");

  fputs("_cupsRasterExecPS(\"\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        ""))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (header.)
  {
    printf("FAIL ()\n");
    errors ++;
  }
  else
    puts("PASS");

  fputs("_cupsRasterExecPS(\"\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        ""))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (header.)
  {
    printf("FAIL ()\n");
    errors ++;
  }
  else
    puts("PASS");

  fputs("_cupsRasterExecPS(\"\"): ", stdout);
  fflush(stdout);

  if (_cupsRasterExecPS(&header, &preferred_bits,
                        ""))
  {
    puts("FAIL (error from function)");
    errors ++;
  }
  else if (header.)
  {
    printf("FAIL ()\n");
    errors ++;
  }
  else
    puts("PASS");
#endif /* 0 */

 /*
  * Test writing...
  */

  fputs("cupsRasterOpen(CUPS_RASTER_WRITE): ", stdout);
  fflush(stdout);

  if ((fp = fopen("test.raster", "wb")) == NULL)
  {
    printf("FAIL (%s)\n", strerror(errno));
    return (1);
  }

  if ((r = cupsRasterOpen(fileno(fp), CUPS_RASTER_WRITE)) == NULL)
  {
    printf("FAIL (%s)\n", strerror(errno));
    fclose(fp);
    return (1);
  }

  puts("PASS");

  for (page = 0; page < 4; page ++)
  {
    memset(&header, 0, sizeof(header));
    header.cupsWidth        = 256;
    header.cupsHeight       = 256;
    header.cupsBytesPerLine = 256;

    if (page & 1)
    {
      header.cupsBytesPerLine *= 2;
      header.cupsColorSpace = CUPS_CSPACE_CMYK;
      header.cupsColorOrder = CUPS_ORDER_CHUNKED;
      header.cupsNumColors  = 4;
    }
    else
    {
      header.cupsColorSpace = CUPS_CSPACE_K;
      header.cupsColorOrder = CUPS_ORDER_BANDED;
      header.cupsNumColors  = 1;
    }

    if (page & 2)
    {
      header.cupsBytesPerLine *= 2;
      header.cupsBitsPerColor = 16;
      header.cupsBitsPerPixel = (page & 1) ? 64 : 16;
    }
    else
    {
      header.cupsBitsPerColor = 8;
      header.cupsBitsPerPixel = (page & 1) ? 32 : 8;
    }

    if (cupsRasterWriteHeader2(r, &header))
      puts("cupsRasterWriteHeader2: PASS");
    else
    {
      puts("cupsRasterWriteHeader2: FAIL");
      errors ++;
    }

    fputs("cupsRasterWritePixels: ", stdout);
    fflush(stdout);

    memset(data, 0, header.cupsBytesPerLine);
    for (y = 0; y < 64; y ++)
      if (!cupsRasterWritePixels(r, data, header.cupsBytesPerLine))
        break;

    if (y < 64)
    {
      puts("FAIL");
      errors ++;
    }
    else
    {
      for (x = 0; x < header.cupsBytesPerLine; x ++)
	data[x] = x;

      for (y = 0; y < 64; y ++)
	if (!cupsRasterWritePixels(r, data, header.cupsBytesPerLine))
	  break;

      if (y < 64)
      {
	puts("FAIL");
	errors ++;
      }
      else
      {
	memset(data, 255, header.cupsBytesPerLine);
	for (y = 0; y < 64; y ++)
	  if (!cupsRasterWritePixels(r, data, header.cupsBytesPerLine))
	    break;

	if (y < 64)
	{
	  puts("FAIL");
	  errors ++;
	}
	else
	{
	  for (x = 0; x < header.cupsBytesPerLine; x ++)
	    data[x] = x / 4;

	  for (y = 0; y < 64; y ++)
	    if (!cupsRasterWritePixels(r, data, header.cupsBytesPerLine))
	      break;

	  if (y < 64)
	  {
	    puts("FAIL");
	    errors ++;
	  }
	  else
	    puts("PASS");
        }
      }
    }
  }

  cupsRasterClose(r);
  fclose(fp);

 /*
  * Test reading...
  */

  fputs("cupsRasterOpen(CUPS_RASTER_READ): ", stdout);
  fflush(stdout);

  if ((fp = fopen("test.raster", "rb")) == NULL)
  {
    printf("FAIL (%s)\n", strerror(errno));
    return (1);
  }

  if ((r = cupsRasterOpen(fileno(fp), CUPS_RASTER_READ)) == NULL)
  {
    printf("FAIL (%s)\n", strerror(errno));
    fclose(fp);
    return (1);
  }

  puts("PASS");

  for (page = 0; page < 4; page ++)
  {
    memset(&expected, 0, sizeof(expected));
    expected.cupsWidth        = 256;
    expected.cupsHeight       = 256;
    expected.cupsBytesPerLine = 256;

    if (page & 1)
    {
      expected.cupsBytesPerLine *= 2;
      expected.cupsColorSpace = CUPS_CSPACE_CMYK;
      expected.cupsColorOrder = CUPS_ORDER_CHUNKED;
      expected.cupsNumColors  = 4;
    }
    else
    {
      expected.cupsColorSpace = CUPS_CSPACE_K;
      expected.cupsColorOrder = CUPS_ORDER_BANDED;
      expected.cupsNumColors  = 1;
    }

    if (page & 2)
    {
      expected.cupsBytesPerLine *= 2;
      expected.cupsBitsPerColor = 16;
      expected.cupsBitsPerPixel = (page & 1) ? 64 : 16;
    }
    else
    {
      expected.cupsBitsPerColor = 8;
      expected.cupsBitsPerPixel = (page & 1) ? 32 : 8;
    }

    fputs("cupsRasterReadHeader2: ", stdout);
    fflush(stdout);

    if (!cupsRasterReadHeader2(r, &header))
    {
      puts("FAIL (read error)");
      errors ++;
      break;
    }

    if (memcmp(&header, &expected, sizeof(header)))
    {
      puts("FAIL (bad page header)");
      errors ++;
      print_changes(&header, &expected);
    }

    fputs("cupsRasterReadPixels: ", stdout);
    fflush(stdout);

    for (y = 0; y < 64; y ++)
    {
      if (!cupsRasterReadPixels(r, data, header.cupsBytesPerLine))
      {
        puts("FAIL (read error)");
	errors ++;
	break;
      }

      if (data[0] != 0 || memcmp(data, data + 1, header.cupsBytesPerLine - 1))
      {
        printf("FAIL (raster line %d corrupt)\n", y);
	errors ++;
	break;
      }
    }

    if (y == 64)
    {
      for (y = 0; y < 64; y ++)
      {
	if (!cupsRasterReadPixels(r, data, header.cupsBytesPerLine))
	{
	  puts("FAIL (read error)");
	  errors ++;
	  break;
	}

	for (x = 0; x < header.cupsBytesPerLine; x ++)
          if (data[x] != (x & 255))
	    break;

	if (x < header.cupsBytesPerLine)
	{
	  printf("FAIL (raster line %d corrupt)\n", y + 64);
	  errors ++;
	  break;
	}
      }

      if (y == 64)
      {
	for (y = 0; y < 64; y ++)
	{
	  if (!cupsRasterReadPixels(r, data, header.cupsBytesPerLine))
	  {
	    puts("FAIL (read error)");
	    errors ++;
	    break;
	  }

	  if (data[0] != 255 || memcmp(data, data + 1, header.cupsBytesPerLine - 1))
          {
	    printf("fail (raster line %d corrupt)\n", y + 128);
	    errors ++;
	    break;
	  }
	}

        if (y == 64)
	{
	  for (y = 0; y < 64; y ++)
	  {
	    if (!cupsRasterReadPixels(r, data, header.cupsBytesPerLine))
	    {
	      puts("FAIL (read error)");
	      errors ++;
	      break;
	    }

	    for (x = 0; x < header.cupsBytesPerLine; x ++)
              if (data[x] != ((x / 4) & 255))
		break;

	    if (x < header.cupsBytesPerLine)
            {
	      printf("FAIL (raster line %d corrupt)\n", y + 192);
	      errors ++;
	      break;
	    }
	  }

	  if (y == 64)
	    puts("PASS");
	}
      }
    }
  }

  cupsRasterClose(r);
  fclose(fp);

  return (errors > 0);
}



/*
 * 'print_changes()' - Print differences in the page header.
 */

static void
print_changes(
    cups_page_header2_t *header,	/* I - Actual page header */
    cups_page_header2_t *expected)	/* I - Expected page header */
{
  int	i;				/* Looping var */


  if (strcmp(header->MediaClass, expected->MediaClass))
    printf("    MediaClass (%s), expected (%s)\n", header->MediaClass,
           expected->MediaClass);

  if (strcmp(header->MediaColor, expected->MediaColor))
    printf("    MediaColor (%s), expected (%s)\n", header->MediaColor,
           expected->MediaColor);

  if (strcmp(header->MediaType, expected->MediaType))
    printf("    MediaType (%s), expected (%s)\n", header->MediaType,
           expected->MediaType);

  if (strcmp(header->OutputType, expected->OutputType))
    printf("    OutputType (%s), expected (%s)\n", header->OutputType,
           expected->OutputType);

  if (header->AdvanceDistance != expected->AdvanceDistance)
    printf("    AdvanceDistance %d, expected %d\n", header->AdvanceDistance,
           expected->AdvanceDistance);

  if (header->AdvanceMedia != expected->AdvanceMedia)
    printf("    AdvanceMedia %d, expected %d\n", header->AdvanceMedia,
           expected->AdvanceMedia);

  if (header->Collate != expected->Collate)
    printf("    Collate %d, expected %d\n", header->Collate,
           expected->Collate);

  if (header->CutMedia != expected->CutMedia)
    printf("    CutMedia %d, expected %d\n", header->CutMedia,
           expected->CutMedia);

  if (header->Duplex != expected->Duplex)
    printf("    Duplex %d, expected %d\n", header->Duplex,
           expected->Duplex);

  if (header->HWResolution[0] != expected->HWResolution[0] ||
      header->HWResolution[1] != expected->HWResolution[1])
    printf("    HWResolution [%d %d], expected [%d %d]\n",
           header->HWResolution[0], header->HWResolution[1],
           expected->HWResolution[0], expected->HWResolution[1]);

  if (memcmp(header->ImagingBoundingBox, expected->ImagingBoundingBox,
             sizeof(header->ImagingBoundingBox)))
    printf("    ImagingBoundingBox [%d %d %d %d], expected [%d %d %d %d]\n",
           header->ImagingBoundingBox[0],
           header->ImagingBoundingBox[1],
           header->ImagingBoundingBox[2],
           header->ImagingBoundingBox[3],
           expected->ImagingBoundingBox[0],
           expected->ImagingBoundingBox[1],
           expected->ImagingBoundingBox[2],
           expected->ImagingBoundingBox[3]);

  if (header->InsertSheet != expected->InsertSheet)
    printf("    InsertSheet %d, expected %d\n", header->InsertSheet,
           expected->InsertSheet);

  if (header->Jog != expected->Jog)
    printf("    Jog %d, expected %d\n", header->Jog,
           expected->Jog);

  if (header->LeadingEdge != expected->LeadingEdge)
    printf("    LeadingEdge %d, expected %d\n", header->LeadingEdge,
           expected->LeadingEdge);

  if (header->Margins[0] != expected->Margins[0] ||
      header->Margins[1] != expected->Margins[1])
    printf("    Margins [%d %d], expected [%d %d]\n",
           header->Margins[0], header->Margins[1],
           expected->Margins[0], expected->Margins[1]);

  if (header->ManualFeed != expected->ManualFeed)
    printf("    ManualFeed %d, expected %d\n", header->ManualFeed,
           expected->ManualFeed);

  if (header->MediaPosition != expected->MediaPosition)
    printf("    MediaPosition %d, expected %d\n", header->MediaPosition,
           expected->MediaPosition);

  if (header->MediaWeight != expected->MediaWeight)
    printf("    MediaWeight %d, expected %d\n", header->MediaWeight,
           expected->MediaWeight);

  if (header->MirrorPrint != expected->MirrorPrint)
    printf("    MirrorPrint %d, expected %d\n", header->MirrorPrint,
           expected->MirrorPrint);

  if (header->NegativePrint != expected->NegativePrint)
    printf("    NegativePrint %d, expected %d\n", header->NegativePrint,
           expected->NegativePrint);

  if (header->NumCopies != expected->NumCopies)
    printf("    NumCopies %d, expected %d\n", header->NumCopies,
           expected->NumCopies);

  if (header->Orientation != expected->Orientation)
    printf("    Orientation %d, expected %d\n", header->Orientation,
           expected->Orientation);

  if (header->OutputFaceUp != expected->OutputFaceUp)
    printf("    OutputFaceUp %d, expected %d\n", header->OutputFaceUp,
           expected->OutputFaceUp);

  if (header->PageSize[0] != expected->PageSize[0] ||
      header->PageSize[1] != expected->PageSize[1])
    printf("    PageSize [%d %d], expected [%d %d]\n",
           header->PageSize[0], header->PageSize[1],
           expected->PageSize[0], expected->PageSize[1]);

  if (header->Separations != expected->Separations)
    printf("    Separations %d, expected %d\n", header->Separations,
           expected->Separations);

  if (header->TraySwitch != expected->TraySwitch)
    printf("    TraySwitch %d, expected %d\n", header->TraySwitch,
           expected->TraySwitch);

  if (header->Tumble != expected->Tumble)
    printf("    Tumble %d, expected %d\n", header->Tumble,
           expected->Tumble);

  if (header->cupsWidth != expected->cupsWidth)
    printf("    cupsWidth %d, expected %d\n", header->cupsWidth,
           expected->cupsWidth);

  if (header->cupsHeight != expected->cupsHeight)
    printf("    cupsHeight %d, expected %d\n", header->cupsHeight,
           expected->cupsHeight);

  if (header->cupsMediaType != expected->cupsMediaType)
    printf("    cupsMediaType %d, expected %d\n", header->cupsMediaType,
           expected->cupsMediaType);

  if (header->cupsBitsPerColor != expected->cupsBitsPerColor)
    printf("    cupsBitsPerColor %d, expected %d\n", header->cupsBitsPerColor,
           expected->cupsBitsPerColor);

  if (header->cupsBitsPerPixel != expected->cupsBitsPerPixel)
    printf("    cupsBitsPerPixel %d, expected %d\n", header->cupsBitsPerPixel,
           expected->cupsBitsPerPixel);

  if (header->cupsBytesPerLine != expected->cupsBytesPerLine)
    printf("    cupsBytesPerLine %d, expected %d\n", header->cupsBytesPerLine,
           expected->cupsBytesPerLine);

  if (header->cupsColorOrder != expected->cupsColorOrder)
    printf("    cupsColorOrder %d, expected %d\n", header->cupsColorOrder,
           expected->cupsColorOrder);

  if (header->cupsColorSpace != expected->cupsColorSpace)
    printf("    cupsColorSpace %d, expected %d\n", header->cupsColorSpace,
           expected->cupsColorSpace);

  if (header->cupsCompression != expected->cupsCompression)
    printf("    cupsCompression %d, expected %d\n", header->cupsCompression,
           expected->cupsCompression);

  if (header->cupsRowCount != expected->cupsRowCount)
    printf("    cupsRowCount %d, expected %d\n", header->cupsRowCount,
           expected->cupsRowCount);

  if (header->cupsRowFeed != expected->cupsRowFeed)
    printf("    cupsRowFeed %d, expected %d\n", header->cupsRowFeed,
           expected->cupsRowFeed);

  if (header->cupsRowStep != expected->cupsRowStep)
    printf("    cupsRowStep %d, expected %d\n", header->cupsRowStep,
           expected->cupsRowStep);

  if (header->cupsNumColors != expected->cupsNumColors)
    printf("    cupsNumColors %d, expected %d\n", header->cupsNumColors,
           expected->cupsNumColors);

  if (header->cupsBorderlessScalingFactor !=
          expected->cupsBorderlessScalingFactor)
    printf("    cupsBorderlessScalingFactor %g, expected %g\n",
           header->cupsBorderlessScalingFactor,
           expected->cupsBorderlessScalingFactor);

  if (header->cupsPageSize[0] != expected->cupsPageSize[0] ||
      header->cupsPageSize[1] != expected->cupsPageSize[1])
    printf("    cupsPageSize [%g %g], expected [%g %g]\n",
           header->cupsPageSize[0], header->cupsPageSize[1],
           expected->cupsPageSize[0], expected->cupsPageSize[1]);

  if (header->cupsImagingBBox[0] != expected->cupsImagingBBox[0] ||
      header->cupsImagingBBox[1] != expected->cupsImagingBBox[1] ||
      header->cupsImagingBBox[2] != expected->cupsImagingBBox[2] ||
      header->cupsImagingBBox[3] != expected->cupsImagingBBox[3])
    printf("    cupsImagingBBox [%g %g %g %g], expected [%g %g %g %g]\n",
           header->cupsImagingBBox[0], header->cupsImagingBBox[1],
           header->cupsImagingBBox[2], header->cupsImagingBBox[3],
           expected->cupsImagingBBox[0], expected->cupsImagingBBox[1],
           expected->cupsImagingBBox[2], expected->cupsImagingBBox[3]);

  for (i = 0; i < 16; i ++)
    if (header->cupsInteger[i] != expected->cupsInteger[i])
      printf("    cupsInteger%d %d, expected %d\n", i, header->cupsInteger[i],
             expected->cupsInteger[i]);

  for (i = 0; i < 16; i ++)
    if (header->cupsReal[i] != expected->cupsReal[i])
      printf("    cupsReal%d %g, expected %g\n", i, header->cupsReal[i],
             expected->cupsReal[i]);

  for (i = 0; i < 16; i ++)
    if (strcmp(header->cupsString[i], expected->cupsString[i]))
      printf("    cupsString%d (%s), expected (%s)\n", i,
	     header->cupsString[i], expected->cupsString[i]);

  if (strcmp(header->cupsMarkerType, expected->cupsMarkerType))
    printf("    cupsMarkerType (%s), expected (%s)\n", header->cupsMarkerType,
           expected->cupsMarkerType);

  if (strcmp(header->cupsRenderingIntent, expected->cupsRenderingIntent))
    printf("    cupsRenderingIntent (%s), expected (%s)\n",
           header->cupsRenderingIntent,
           expected->cupsRenderingIntent);

  if (strcmp(header->cupsPageSizeName, expected->cupsPageSizeName))
    printf("    cupsPageSizeName (%s), expected (%s)\n",
           header->cupsPageSizeName,
           expected->cupsPageSizeName);
}


/*
 * End of "$Id: testraster.c 6158 2006-12-17 01:44:21Z mike $".
 */

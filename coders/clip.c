/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                          CCCC  L      IIIII  PPPP                           %
%                         C      L        I    P   P                          %
%                         C      L        I    PPPP                           %
%                         C      L        I    P                              %
%                          CCCC  LLLLL  IIIII  P                              %
%                                                                             %
%                                                                             %
%                        Write Clip Mask To MIFF File.                        %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                 July 1992                                   %
%                                                                             %
%                                                                             %
%  Copyright 1999-2011 ImageMagick Studio LLC, a non-profit organization      %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    http://www.imagemagick.org/script/license.php                            %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/

/*
  Include declarations.
*/
#include "MagickCore/studio.h"
#include "MagickCore/blob.h"
#include "MagickCore/blob-private.h"
#include "MagickCore/constitute.h"
#include "MagickCore/exception.h"
#include "MagickCore/exception-private.h"
#include "MagickCore/magick.h"
#include "MagickCore/memory_.h"
#include "MagickCore/monitor.h"
#include "MagickCore/monitor-private.h"
#include "MagickCore/quantum-private.h"
#include "MagickCore/static.h"
#include "MagickCore/string_.h"
#include "MagickCore/module.h"

/*
  Forward declarations.
*/
static MagickBooleanType
  WriteCLIPImage(const ImageInfo *,Image *,ExceptionInfo *);

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r C L I P I m a g e                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RegisterCLIPImage() adds attributes for the CLIP image format to
%  the list of supported formats.  The attributes include the image format
%  tag, a method to read and/or write the format, whether the format
%  supports the saving of more than one frame to the same file or blob,
%  whether the format supports native in-memory I/O, and a brief
%  description of the format.
%
%  The format of the RegisterCLIPImage method is:
%
%      size_t RegisterCLIPImage(void)
%
*/
ModuleExport size_t RegisterCLIPImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("CLIP");
  entry->encoder=(EncodeImageHandler *) WriteCLIPImage;
  entry->description=ConstantString("Image Clip Mask");
  entry->module=ConstantString("CLIP");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r C L I P I m a g e                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  UnregisterCLIPImage() removes format registrations made by the
%  CLIP module from the list of supported formats.
%
%  The format of the UnregisterCLIPImage method is:
%
%      UnregisterCLIPImage(void)
%
*/
ModuleExport void UnregisterCLIPImage(void)
{
  (void) UnregisterMagickInfo("CLIP");
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e C L I P I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  WriteCLIPImage() writes an image of clip bytes to a file.  It consists of
%  data from the clip mask of the image.
%
%  The format of the WriteCLIPImage method is:
%
%      MagickBooleanType WriteCLIPImage(const ImageInfo *image_info,
%        Image *image,ExceptionInfo *exception)
%
%  A description of each parameter follows.
%
%    o image_info: the image info.
%
%    o image:  The image.
%
%    o exception: return any errors or warnings in this structure.
%
*/
static MagickBooleanType WriteCLIPImage(const ImageInfo *image_info,
  Image *image,ExceptionInfo *exception)
{
  Image
    *clip_image;

  ImageInfo
    *write_info;

  MagickBooleanType
    status;

  if (image->clip_mask == (Image *) NULL)
    (void) ClipImage(image);
  if (image->clip_mask == (Image *) NULL)
    ThrowWriterException(CoderError,"ImageDoesNotHaveAClipMask");
  clip_image=CloneImage(image->clip_mask,0,0,MagickTrue,exception);
  if (clip_image == (Image *) NULL)
    return(MagickFalse);
  (void) SetImageType(clip_image,TrueColorType);
  (void) CopyMagickString(clip_image->filename,image->filename,MaxTextExtent);
  write_info=CloneImageInfo(image_info);
  (void) SetImageInfo(write_info,1,exception);
  if (LocaleCompare(write_info->magick,"CLIP") == 0)
    (void) FormatLocaleString(clip_image->filename,MaxTextExtent,"miff:%s",
      write_info->filename);
  status=WriteImage(write_info,clip_image,exception);
  clip_image=DestroyImage(clip_image);
  write_info=DestroyImageInfo(write_info);
  return(status);
}

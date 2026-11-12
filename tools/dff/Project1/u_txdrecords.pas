unit u_txdrecords;

interface

type
  // Descriptive names; layout established by the native 124-byte stream read.
  TNativeTextureHeader = packed record
    Prefix: array[0..43] of Byte;
    Name: array[0..31] of AnsiChar;
    Mask: array[0..31] of AnsiChar;
    RasterFormat: Cardinal;
    D3DFormat: array[0..3] of AnsiChar;
    Width, Height: Word;
    Depth, MipMapCount, RasterType, Flags: Byte;
  end;
  // 36-byte elements in the native texture table.
  TTextureEntry = record
    TextureID: Cardinal;
    Name: array[0..31] of AnsiChar;
  end;

implementation

end.

/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EOLN = 258,
     PATH = 259,
     L_ACCOLADE = 260,
     R_ACCOLADE = 261,
     L_PARENTHESE = 262,
     R_PARENTHESE = 263,
     L_BRACKET = 264,
     R_BRACKET = 265,
     L_ANGLEBRACKET = 266,
     R_ANGLEBRACKET = 267,
     OR = 268,
     ASSIGN = 269,
     COMMA = 270,
     UNDERSCORE = 271,
     ASTERIK = 272,
     FORWARDSLASH = 273,
     PLUS = 274,
     MINUS = 275,
     N_HEX = 276,
     N_FLOAT = 277,
     N_INT = 278,
     N_STRING = 279,
     N_QUOTE = 280,
     N_BOOL = 281,
     N_VERSION = 282,
     NSFSHADER = 283,
     ARTIST = 284,
     HIDDEN = 285,
     SAVE = 286,
     ATTRIBUTES = 287,
     GLOBALATTRIBUTES = 288,
     ATTRIB = 289,
     ATTRIB_BOOL = 290,
     ATTRIB_STRING = 291,
     ATTRIB_UINT = 292,
     ATTRIB_FLOAT = 293,
     ATTRIB_POINT2 = 294,
     ATTRIB_POINT3 = 295,
     ATTRIB_POINT4 = 296,
     ATTRIB_MATRIX3 = 297,
     ATTRIB_TRANSFORM = 298,
     ATTRIB_COLOR = 299,
     ATTRIB_TEXTURE = 300,
     PACKINGDEF = 301,
     PD_STREAM = 302,
     PD_FIXEDFUNCTION = 303,
     PDP_POSITION = 304,
     PDP_BLENDWEIGHTS = 305,
     PDP_BLENDINDICES = 306,
     PDP_NORMAL = 307,
     PDP_POINTSIZE = 308,
     PDP_COLOR = 309,
     PDP_COLOR2 = 310,
     PDP_TEXCOORD0 = 311,
     PDP_TEXCOORD1 = 312,
     PDP_TEXCOORD2 = 313,
     PDP_TEXCOORD3 = 314,
     PDP_TEXCOORD4 = 315,
     PDP_TEXCOORD5 = 316,
     PDP_TEXCOORD6 = 317,
     PDP_TEXCOORD7 = 318,
     PDP_POSITION2 = 319,
     PDP_NORMAL2 = 320,
     PDP_TANGENT = 321,
     PDP_BINORMAL = 322,
     PDP_EXTRADATA = 323,
     PDT_FLOAT1 = 324,
     PDT_FLOAT2 = 325,
     PDT_FLOAT3 = 326,
     PDT_FLOAT4 = 327,
     PDT_UBYTECOLOR = 328,
     PDT_SHORT1 = 329,
     PDT_SHORT2 = 330,
     PDT_SHORT3 = 331,
     PDT_SHORT4 = 332,
     PDT_UBYTE4 = 333,
     PDT_NORMSHORT1 = 334,
     PDT_NORMSHORT2 = 335,
     PDT_NORMSHORT3 = 336,
     PDT_NORMSHORT4 = 337,
     PDT_NORMPACKED3 = 338,
     PDT_PBYTE1 = 339,
     PDT_PBYTE2 = 340,
     PDT_PBYTE3 = 341,
     PDT_PBYTE4 = 342,
     PDT_FLOAT2H = 343,
     PDT_NORMUBYTE4 = 344,
     PDT_NORMUSHORT2 = 345,
     PDT_NORMUSHORT4 = 346,
     PDT_UDEC3 = 347,
     PDT_NORMDEC3 = 348,
     PDT_FLOAT16_2 = 349,
     PDT_FLOAT16_4 = 350,
     PDTESS_DEFAULT = 351,
     PDTESS_PARTIALU = 352,
     PDTESS_PARTIALV = 353,
     PDTESS_CROSSUV = 354,
     PDTESS_UV = 355,
     PDTESS_LOOKUP = 356,
     PDTESS_LOOKUPPRESAMPLED = 357,
     PDU_POSITION = 358,
     PDU_BLENDWEIGHT = 359,
     PDU_BLENDINDICES = 360,
     PDU_NORMAL = 361,
     PDU_PSIZE = 362,
     PDU_TEXCOORD = 363,
     PDU_TANGENT = 364,
     PDU_BINORMAL = 365,
     PDU_TESSFACTOR = 366,
     PDU_POSITIONT = 367,
     PDU_COLOR = 368,
     PDU_FOG = 369,
     PDU_DEPTH = 370,
     PDU_SAMPLE = 371,
     RENDERSTATES = 372,
     CMDEFINED = 373,
     CMATTRIBUTE = 374,
     CMCONSTANT = 375,
     CMGLOBAL = 376,
     CMOPERATOR = 377,
     VSCONSTANTMAP = 378,
     VSPROGRAM = 379,
     GSCONSTANTMAP = 380,
     GSPROGRAM = 381,
     PSCONSTANTMAP = 382,
     PSPROGRAM = 383,
     PROGRAM = 384,
     ENTRYPOINT = 385,
     SHADERTARGET = 386,
     SOFTWAREVP = 387,
     BONEMATRIX3 = 388,
     BONEMATRIX4 = 389,
     SKINBONEMATRIX3 = 390,
     SKINBONEMATRIX4 = 391,
     REQUIREMENTS = 392,
     VSVERSION = 393,
     GSVERSION = 394,
     PSVERSION = 395,
     USERVERSION = 396,
     PLATFORM = 397,
     BONESPERPARTITION = 398,
     BINORMALTANGENTMETHOD = 399,
     BINORMALTANGENTUVSOURCE = 400,
     NBTMETHOD_NONE = 401,
     NBTMETHOD_NI = 402,
     NBTMETHOD_MAX = 403,
     NBTMETHOD_ATI = 404,
     USERDEFINEDDATA = 405,
     IMPLEMENTATION = 406,
     CLASSNAME = 407,
     PASS = 408,
     STAGE = 409,
     TSS_TEXTURE = 410,
     TSS_COLOROP = 411,
     TSS_COLORARG0 = 412,
     TSS_COLORARG1 = 413,
     TSS_COLORARG2 = 414,
     TSS_ALPHAOP = 415,
     TSS_ALPHAARG0 = 416,
     TSS_ALPHAARG1 = 417,
     TSS_ALPHAARG2 = 418,
     TSS_RESULTARG = 419,
     TSS_CONSTANT_DEPRECATED = 420,
     TSS_BUMPENVMAT00 = 421,
     TSS_BUMPENVMAT01 = 422,
     TSS_BUMPENVMAT10 = 423,
     TSS_BUMPENVMAT11 = 424,
     TSS_BUMPENVLSCALE = 425,
     TSS_BUMPENVLOFFSET = 426,
     TSS_TEXCOORDINDEX = 427,
     TSS_TEXTURETRANSFORMFLAGS = 428,
     TSS_TEXTRANSMATRIX = 429,
     TTFF_DISABLE = 430,
     TTFF_COUNT1 = 431,
     TTFF_COUNT2 = 432,
     TTFF_COUNT3 = 433,
     TTFF_COUNT4 = 434,
     TTFF_PROJECTED = 435,
     PROJECTED = 436,
     USEMAPINDEX = 437,
     INVERSE = 438,
     TRANSPOSE = 439,
     TTSRC_GLOBAL = 440,
     TTSRC_CONSTANT = 441,
     TT_WORLD_PARALLEL = 442,
     TT_WORLD_PERSPECTIVE = 443,
     TT_WORLD_SPHERE_MAP = 444,
     TT_CAMERA_SPHERE_MAP = 445,
     TT_SPECULAR_CUBE_MAP = 446,
     TT_DIFFUSE_CUBE_MAP = 447,
     TCI_PASSTHRU = 448,
     TCI_CAMERASPACENORMAL = 449,
     TCI_CAMERASPACEPOSITION = 450,
     TCI_CAMERASPACEREFLECT = 451,
     TCI_SPHEREMAP = 452,
     TOP_DISABLE = 453,
     TOP_SELECTARG1 = 454,
     TOP_SELECTARG2 = 455,
     TOP_MODULATE = 456,
     TOP_MODULATE2X = 457,
     TOP_MODULATE4X = 458,
     TOP_ADD = 459,
     TOP_ADDSIGNED = 460,
     TOP_ADDSIGNED2X = 461,
     TOP_SUBTRACT = 462,
     TOP_ADDSMOOTH = 463,
     TOP_BLENDDIFFUSEALPHA = 464,
     TOP_BLENDTEXTUREALPHA = 465,
     TOP_BLENDFACTORALPHA = 466,
     TOP_BLENDTEXTUREALPHAPM = 467,
     TOP_BLENDCURRENTALPHA = 468,
     TOP_PREMODULATE = 469,
     TOP_MODULATEALPHA_ADDCOLOR = 470,
     TOP_MODULATECOLOR_ADDALPHA = 471,
     TOP_MODULATEINVALPHA_ADDCOLOR = 472,
     TOP_MODULATEINVCOLOR_ADDALPHA = 473,
     TOP_BUMPENVMAP = 474,
     TOP_BUMPENVMAPLUMINANCE = 475,
     TOP_DOTPRODUCT3 = 476,
     TOP_MULTIPLYADD = 477,
     TOP_LERP = 478,
     TA_CURRENT = 479,
     TA_DIFFUSE = 480,
     TA_SELECTMASK = 481,
     TA_SPECULAR = 482,
     TA_TEMP = 483,
     TA_TEXTURE = 484,
     TA_TFACTOR = 485,
     TA_ALPHAREPLICATE = 486,
     TA_COMPLEMENT = 487,
     NTM_BASE = 488,
     NTM_DARK = 489,
     NTM_DETAIL = 490,
     NTM_GLOSS = 491,
     NTM_GLOW = 492,
     NTM_BUMP = 493,
     NTM_NORMAL = 494,
     NTM_PARALLAX = 495,
     NTM_DECAL = 496,
     SAMPLER = 497,
     TSAMP_ADDRESSU = 498,
     TSAMP_ADDRESSV = 499,
     TSAMP_ADDRESSW = 500,
     TSAMP_BORDERCOLOR = 501,
     TSAMP_MAGFILTER = 502,
     TSAMP_MINFILTER = 503,
     TSAMP_MIPFILTER = 504,
     TSAMP_MIPMAPLODBIAS = 505,
     TSAMP_MAXMIPLEVEL = 506,
     TSAMP_MAXANISOTROPY = 507,
     TSAMP_SRGBTEXTURE = 508,
     TSAMP_ELEMENTINDEX = 509,
     TSAMP_DMAPOFFSET = 510,
     TSAMP_ALPHAKILL_DEPRECATED = 511,
     TSAMP_COLORKEYOP_DEPRECATED = 512,
     TSAMP_COLORSIGN_DEPRECATED = 513,
     TSAMP_COLORKEYCOLOR_DEPRECATED = 514,
     TADDR_WRAP = 515,
     TADDR_MIRROR = 516,
     TADDR_CLAMP = 517,
     TADDR_BORDER = 518,
     TADDR_MIRRORONCE = 519,
     TADDR_CLAMPTOEDGE_DEPRECATED = 520,
     TEXF_NONE = 521,
     TEXF_POINT = 522,
     TEXF_LINEAR = 523,
     TEXF_ANISOTROPIC = 524,
     TEXF_PYRAMIDALQUAD = 525,
     TEXF_GAUSSIANQUAD = 526,
     TEXF_FLATCUBIC_DEPRECATED = 527,
     TEXF_GAUSSIANCUBIC_DEPRECATED = 528,
     TEXF_QUINCUNX_DEPRECATED = 529,
     TEXF_MAX_DEPRECATED = 530,
     TAK_DISABLE_DEPRECATED = 531,
     TAK_ENABLE_DEPRECATED = 532,
     TCKOP_DISABLE_DEPRECATED = 533,
     TCKOP_ALPHA_DEPRECATED = 534,
     TCKOP_RGBA_DEPRECATED = 535,
     TCKOP_KILL_DEPRECATED = 536,
     TEXTURE = 537,
     TEXTURE_SOURCE = 538,
     OBJECTS = 539,
     EFFECT_GENERALLIGHT = 540,
     EFFECT_POINTLIGHT = 541,
     EFFECT_DIRECTIONALLIGHT = 542,
     EFFECT_SPOTLIGHT = 543,
     EFFECT_SHADOWPOINTLIGHT = 544,
     EFFECT_SHADOWDIRECTIONALLIGHT = 545,
     EFFECT_SHADOWSPOTLIGHT = 546,
     EFFECT = 547,
     EFFECT_ENVIRONMENTMAP = 548,
     EFFECT_PROJECTEDSHADOWMAP = 549,
     EFFECT_PROJECTEDLIGHTMAP = 550,
     EFFECT_FOGMAP = 551,
     USEMAPVALUE = 552,
     CMOBJECT = 553
   };
#endif
/* Tokens.  */
#define EOLN 258
#define PATH 259
#define L_ACCOLADE 260
#define R_ACCOLADE 261
#define L_PARENTHESE 262
#define R_PARENTHESE 263
#define L_BRACKET 264
#define R_BRACKET 265
#define L_ANGLEBRACKET 266
#define R_ANGLEBRACKET 267
#define OR 268
#define ASSIGN 269
#define COMMA 270
#define UNDERSCORE 271
#define ASTERIK 272
#define FORWARDSLASH 273
#define PLUS 274
#define MINUS 275
#define N_HEX 276
#define N_FLOAT 277
#define N_INT 278
#define N_STRING 279
#define N_QUOTE 280
#define N_BOOL 281
#define N_VERSION 282
#define NSFSHADER 283
#define ARTIST 284
#define HIDDEN 285
#define SAVE 286
#define ATTRIBUTES 287
#define GLOBALATTRIBUTES 288
#define ATTRIB 289
#define ATTRIB_BOOL 290
#define ATTRIB_STRING 291
#define ATTRIB_UINT 292
#define ATTRIB_FLOAT 293
#define ATTRIB_POINT2 294
#define ATTRIB_POINT3 295
#define ATTRIB_POINT4 296
#define ATTRIB_MATRIX3 297
#define ATTRIB_TRANSFORM 298
#define ATTRIB_COLOR 299
#define ATTRIB_TEXTURE 300
#define PACKINGDEF 301
#define PD_STREAM 302
#define PD_FIXEDFUNCTION 303
#define PDP_POSITION 304
#define PDP_BLENDWEIGHTS 305
#define PDP_BLENDINDICES 306
#define PDP_NORMAL 307
#define PDP_POINTSIZE 308
#define PDP_COLOR 309
#define PDP_COLOR2 310
#define PDP_TEXCOORD0 311
#define PDP_TEXCOORD1 312
#define PDP_TEXCOORD2 313
#define PDP_TEXCOORD3 314
#define PDP_TEXCOORD4 315
#define PDP_TEXCOORD5 316
#define PDP_TEXCOORD6 317
#define PDP_TEXCOORD7 318
#define PDP_POSITION2 319
#define PDP_NORMAL2 320
#define PDP_TANGENT 321
#define PDP_BINORMAL 322
#define PDP_EXTRADATA 323
#define PDT_FLOAT1 324
#define PDT_FLOAT2 325
#define PDT_FLOAT3 326
#define PDT_FLOAT4 327
#define PDT_UBYTECOLOR 328
#define PDT_SHORT1 329
#define PDT_SHORT2 330
#define PDT_SHORT3 331
#define PDT_SHORT4 332
#define PDT_UBYTE4 333
#define PDT_NORMSHORT1 334
#define PDT_NORMSHORT2 335
#define PDT_NORMSHORT3 336
#define PDT_NORMSHORT4 337
#define PDT_NORMPACKED3 338
#define PDT_PBYTE1 339
#define PDT_PBYTE2 340
#define PDT_PBYTE3 341
#define PDT_PBYTE4 342
#define PDT_FLOAT2H 343
#define PDT_NORMUBYTE4 344
#define PDT_NORMUSHORT2 345
#define PDT_NORMUSHORT4 346
#define PDT_UDEC3 347
#define PDT_NORMDEC3 348
#define PDT_FLOAT16_2 349
#define PDT_FLOAT16_4 350
#define PDTESS_DEFAULT 351
#define PDTESS_PARTIALU 352
#define PDTESS_PARTIALV 353
#define PDTESS_CROSSUV 354
#define PDTESS_UV 355
#define PDTESS_LOOKUP 356
#define PDTESS_LOOKUPPRESAMPLED 357
#define PDU_POSITION 358
#define PDU_BLENDWEIGHT 359
#define PDU_BLENDINDICES 360
#define PDU_NORMAL 361
#define PDU_PSIZE 362
#define PDU_TEXCOORD 363
#define PDU_TANGENT 364
#define PDU_BINORMAL 365
#define PDU_TESSFACTOR 366
#define PDU_POSITIONT 367
#define PDU_COLOR 368
#define PDU_FOG 369
#define PDU_DEPTH 370
#define PDU_SAMPLE 371
#define RENDERSTATES 372
#define CMDEFINED 373
#define CMATTRIBUTE 374
#define CMCONSTANT 375
#define CMGLOBAL 376
#define CMOPERATOR 377
#define VSCONSTANTMAP 378
#define VSPROGRAM 379
#define GSCONSTANTMAP 380
#define GSPROGRAM 381
#define PSCONSTANTMAP 382
#define PSPROGRAM 383
#define PROGRAM 384
#define ENTRYPOINT 385
#define SHADERTARGET 386
#define SOFTWAREVP 387
#define BONEMATRIX3 388
#define BONEMATRIX4 389
#define SKINBONEMATRIX3 390
#define SKINBONEMATRIX4 391
#define REQUIREMENTS 392
#define VSVERSION 393
#define GSVERSION 394
#define PSVERSION 395
#define USERVERSION 396
#define PLATFORM 397
#define BONESPERPARTITION 398
#define BINORMALTANGENTMETHOD 399
#define BINORMALTANGENTUVSOURCE 400
#define NBTMETHOD_NONE 401
#define NBTMETHOD_NI 402
#define NBTMETHOD_MAX 403
#define NBTMETHOD_ATI 404
#define USERDEFINEDDATA 405
#define IMPLEMENTATION 406
#define CLASSNAME 407
#define PASS 408
#define STAGE 409
#define TSS_TEXTURE 410
#define TSS_COLOROP 411
#define TSS_COLORARG0 412
#define TSS_COLORARG1 413
#define TSS_COLORARG2 414
#define TSS_ALPHAOP 415
#define TSS_ALPHAARG0 416
#define TSS_ALPHAARG1 417
#define TSS_ALPHAARG2 418
#define TSS_RESULTARG 419
#define TSS_CONSTANT_DEPRECATED 420
#define TSS_BUMPENVMAT00 421
#define TSS_BUMPENVMAT01 422
#define TSS_BUMPENVMAT10 423
#define TSS_BUMPENVMAT11 424
#define TSS_BUMPENVLSCALE 425
#define TSS_BUMPENVLOFFSET 426
#define TSS_TEXCOORDINDEX 427
#define TSS_TEXTURETRANSFORMFLAGS 428
#define TSS_TEXTRANSMATRIX 429
#define TTFF_DISABLE 430
#define TTFF_COUNT1 431
#define TTFF_COUNT2 432
#define TTFF_COUNT3 433
#define TTFF_COUNT4 434
#define TTFF_PROJECTED 435
#define PROJECTED 436
#define USEMAPINDEX 437
#define INVERSE 438
#define TRANSPOSE 439
#define TTSRC_GLOBAL 440
#define TTSRC_CONSTANT 441
#define TT_WORLD_PARALLEL 442
#define TT_WORLD_PERSPECTIVE 443
#define TT_WORLD_SPHERE_MAP 444
#define TT_CAMERA_SPHERE_MAP 445
#define TT_SPECULAR_CUBE_MAP 446
#define TT_DIFFUSE_CUBE_MAP 447
#define TCI_PASSTHRU 448
#define TCI_CAMERASPACENORMAL 449
#define TCI_CAMERASPACEPOSITION 450
#define TCI_CAMERASPACEREFLECT 451
#define TCI_SPHEREMAP 452
#define TOP_DISABLE 453
#define TOP_SELECTARG1 454
#define TOP_SELECTARG2 455
#define TOP_MODULATE 456
#define TOP_MODULATE2X 457
#define TOP_MODULATE4X 458
#define TOP_ADD 459
#define TOP_ADDSIGNED 460
#define TOP_ADDSIGNED2X 461
#define TOP_SUBTRACT 462
#define TOP_ADDSMOOTH 463
#define TOP_BLENDDIFFUSEALPHA 464
#define TOP_BLENDTEXTUREALPHA 465
#define TOP_BLENDFACTORALPHA 466
#define TOP_BLENDTEXTUREALPHAPM 467
#define TOP_BLENDCURRENTALPHA 468
#define TOP_PREMODULATE 469
#define TOP_MODULATEALPHA_ADDCOLOR 470
#define TOP_MODULATECOLOR_ADDALPHA 471
#define TOP_MODULATEINVALPHA_ADDCOLOR 472
#define TOP_MODULATEINVCOLOR_ADDALPHA 473
#define TOP_BUMPENVMAP 474
#define TOP_BUMPENVMAPLUMINANCE 475
#define TOP_DOTPRODUCT3 476
#define TOP_MULTIPLYADD 477
#define TOP_LERP 478
#define TA_CURRENT 479
#define TA_DIFFUSE 480
#define TA_SELECTMASK 481
#define TA_SPECULAR 482
#define TA_TEMP 483
#define TA_TEXTURE 484
#define TA_TFACTOR 485
#define TA_ALPHAREPLICATE 486
#define TA_COMPLEMENT 487
#define NTM_BASE 488
#define NTM_DARK 489
#define NTM_DETAIL 490
#define NTM_GLOSS 491
#define NTM_GLOW 492
#define NTM_BUMP 493
#define NTM_NORMAL 494
#define NTM_PARALLAX 495
#define NTM_DECAL 496
#define SAMPLER 497
#define TSAMP_ADDRESSU 498
#define TSAMP_ADDRESSV 499
#define TSAMP_ADDRESSW 500
#define TSAMP_BORDERCOLOR 501
#define TSAMP_MAGFILTER 502
#define TSAMP_MINFILTER 503
#define TSAMP_MIPFILTER 504
#define TSAMP_MIPMAPLODBIAS 505
#define TSAMP_MAXMIPLEVEL 506
#define TSAMP_MAXANISOTROPY 507
#define TSAMP_SRGBTEXTURE 508
#define TSAMP_ELEMENTINDEX 509
#define TSAMP_DMAPOFFSET 510
#define TSAMP_ALPHAKILL_DEPRECATED 511
#define TSAMP_COLORKEYOP_DEPRECATED 512
#define TSAMP_COLORSIGN_DEPRECATED 513
#define TSAMP_COLORKEYCOLOR_DEPRECATED 514
#define TADDR_WRAP 515
#define TADDR_MIRROR 516
#define TADDR_CLAMP 517
#define TADDR_BORDER 518
#define TADDR_MIRRORONCE 519
#define TADDR_CLAMPTOEDGE_DEPRECATED 520
#define TEXF_NONE 521
#define TEXF_POINT 522
#define TEXF_LINEAR 523
#define TEXF_ANISOTROPIC 524
#define TEXF_PYRAMIDALQUAD 525
#define TEXF_GAUSSIANQUAD 526
#define TEXF_FLATCUBIC_DEPRECATED 527
#define TEXF_GAUSSIANCUBIC_DEPRECATED 528
#define TEXF_QUINCUNX_DEPRECATED 529
#define TEXF_MAX_DEPRECATED 530
#define TAK_DISABLE_DEPRECATED 531
#define TAK_ENABLE_DEPRECATED 532
#define TCKOP_DISABLE_DEPRECATED 533
#define TCKOP_ALPHA_DEPRECATED 534
#define TCKOP_RGBA_DEPRECATED 535
#define TCKOP_KILL_DEPRECATED 536
#define TEXTURE 537
#define TEXTURE_SOURCE 538
#define OBJECTS 539
#define EFFECT_GENERALLIGHT 540
#define EFFECT_POINTLIGHT 541
#define EFFECT_DIRECTIONALLIGHT 542
#define EFFECT_SPOTLIGHT 543
#define EFFECT_SHADOWPOINTLIGHT 544
#define EFFECT_SHADOWDIRECTIONALLIGHT 545
#define EFFECT_SHADOWSPOTLIGHT 546
#define EFFECT 547
#define EFFECT_ENVIRONMENTMAP 548
#define EFFECT_PROJECTEDSHADOWMAP 549
#define EFFECT_PROJECTEDLIGHTMAP 550
#define EFFECT_FOGMAP 551
#define USEMAPVALUE 552
#define CMOBJECT 553




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)

typedef union YYSTYPE {
    float fval;
    int   ival;
    char* sval;
    unsigned int uival;
    unsigned long  dword;
    unsigned short word;
    unsigned char  byte;
    bool bval;
    unsigned int vers;
} YYSTYPE;
/* Line 1447 of yacc.c.  */

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE NSFParserlval;



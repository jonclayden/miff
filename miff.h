/* ============================================================================
    MIFF (medical image file format) v1.0 draft definition
    Created by Jon Clayden <jon.clayden+miff@gmail.com>, September 2011
============================================================================ */

typedef struct {
    char        magic_number[8];        /* Fixed to be "MedImag\0" (hex 4d 65 64 49 6d 61 67 00). */
    short       major_version;          /* Currently 1. */
    short       minor_version;          /* Currently 0. */
    short       data_type;              /* See note 2. */
    short       representation;         /* See note 3. */
    double      voxels_stored;          /* See note 3. */
    int         data_offset;            /* See note 4. */
    short       space_unit;             /* See note 5. */
    short       time_unit;              /* See note 5. */
    int         image_dims[6];          /* See note 6. */
    double      voxel_dims[6];          /* See note 5. */
    double      scale_slope;            /* See note 7. */
    double      scale_intercept;        /* See note 7. */
    double      window_min;             /* See note 8. */
    double      window_max;             /* See note 8. */
    double      euler_angles[3];        /* See note 9. */
    short       handedness;             /* See note 9. */
    char        description[30];        /* Free text description, using UTF-8 encoding. */
} miff_header;

typedef struct {
    char        label[20];              /* See note 10. */
    char        value[60];              /* See note 11. */
} miff_metadata;

/* ============================================================================

# NOTES:

1. MIFF files are always stored using big-endian byte ordering.

2. The "data_type" field describes the size and basic type (integer or
    floating-point) of the voxel data stored with the image. Constants
    representing the allowable types are defined below.

3. The "representation" field indicates exactly what data are stored in the
    file, and in what order. If the encoding is MIFF_REPRESENT_CARTESIAN, every
    voxel's value is stored in sequence, with the first dimension moving
    fastest, and the last dimension moving slowest. If the encoding is
    MIFF_REPRESENT_COORDLIST, a list of coordinates of nonzero voxels is stored
    as (32-bit signed) integers at the beginning of the data section of the
    file, followed by a list of their values, using whatever data type is
    indicated by the "data_type" field. This representation is more compact for
    sparse images. In this case, the actual number of voxels stored is given
    in the "voxels_stored" field. If this value is (strictly) less than 1, it
    is taken to indicate the proportion of voxels which are nonzero.(*)

4. The "data_offset" field gives the offset, in bytes, of the first data value
    in the file. This is a 32-bit integer, allowing for metadata sizes up to
    2 GB.

5. The "space_unit" and "time_unit" fields give the units of the voxel spacings
    (the "voxel_dims" field) along space and time dimensions, respectively.
    Constants representing the allowable units are defined below. The first
    three dimensions are taken to be spatial, the fourth temporal, and the
    fifth and sixth have no fixed interpretation.

6. The "image_dims" field gives the number of voxels in the image in each
    dimension. A value of zero indicates that the associated dimension is not
    relevant, but zeroes may not appear before the last relevant dimension.
    A 4D image with no time dimension may therefore have "image_dims" of
    (128,128,128,1,16,0), but not (128,128,128,0,16,0). Note that the product
    of the (nonzero) dimensions is not necessarily equal to the number of
    voxel values stored - see note 3.

7. The "scale_slope" and "scale_intercept" fields are for data scaling. The
    final voxel values are the stored values multiplied by the slope, plus the
    intercept.

8. The "window_min" and "window_max" fields are hints for the range of the data
    values. If they are not both zero, they may be used by visualisation
    software to set the intensity scale.

9. The basic storage convention for MIFF files is RAS (with "handedness" set to
    +1) or LAS (with "handedness" set to -1). Implementors should use one of
    these conventions where possible, but it is acknowledged that resampling
    a rotated image may be undesirable, and therefore the "euler_angles" field
    may be used to indicate that the data are oblique to the standard axes.
    The angles are given in the pitch-roll-yaw convention, so
    
        a = euler_angles[0] (pitch)
        b = euler_angles[1] (roll)
        c = euler_angles[2] (yaw)
    
    A rotation matrix, R, may then be reconstructed as
    
            [ 1     0       0    ] [ cos(a)  0  -sin(a) ] [  cos(c)  sin(c)  0 ]
        R = [ 0   cos(b)  sin(b) ] [   0     1     0    ] [ -sin(c)  cos(c)  0 ]
            [ 0  -sin(b)  cos(b) ] [ sin(a)  0   cos(a) ] [    0       0     1 ]
    
    R will then rotate the standard frame to that of the data, and the inverse
    of R will do the opposite. The Euler angles are specified in degrees to
    avoid rounding error at boundary values, and may each take any value
    between -45 and +45. This (deliberately) does not allow for conventions
    such as LIA - such data should be reordered to RAS or LAS.

10. Metadata labels consist of UTF-8 encoded free text, and may therefore take
    any value. However, names of the form ".DCM.gggg.eeee", where "gggg" and
    "eeee" each represent four hexadecimal digits, are taken to represent the
    corresponding DICOM tag. (The remaining 6 characters of the label are
    ignored if the first 14 have this format.) Use of this mechanism is highly
    recommended for portability when representing of the (many) quantities for
    which there is a DICOM tag, such as echo time or scan date. The same label
    may appear multiple times, in which case the values would generally be
    concatenated.

11. Metadata values are UTF-8 encoded free text strings. If the corresponding
    label indicates a DICOM tag number (see note 10), then the DICOM
    convention of separating multiple values with a backslash ("\") is
    followed.
  
(*) Implementation detail: a "double", rather than 64-bit integer, is used for
    the "voxels_stored" field, since this type is more widely available across
    platforms. Double-precision floating point numbers can store integers
    exactly up to 2^53 (about 9e+15).

============================================================================ */

#define MIFF_DATATYPE_UNDEFINED     0
#define MIFF_DATATYPE_UINT8         1
#define MIFF_DATATYPE_INT16         2
#define MIFF_DATATYPE_INT32         3
#define MIFF_DATATYPE_INT64         4
#define MIFF_DATATYPE_FLOAT32       5
#define MIFF_DATATYPE_FLOAT64       6

#define MIFF_REPRESENT_CARTESIAN    0
#define MIFF_REPRESENT_COORDLIST    1

#define MIFF_UNIT_UNDEFINED         0
#define MIFF_UNIT_MICRON            1
#define MIFF_UNIT_MILLIMETRE        2
#define MIFF_UNIT_MILLIMETER        2
#define MIFF_UNIT_METRE             3
#define MIFF_UNIT_METER             3
#define MIFF_UNIT_MICROSECOND      16
#define MIFF_UNIT_MILLISECOND      17
#define MIFF_UNIT_SECOND           18

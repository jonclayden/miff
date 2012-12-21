/* ============================================================================
    MIFF (medical image file format) v1.0 draft definition
    Created by Jon Clayden <jon.clayden+miff@gmail.com>, September 2011
============================================================================ */

/* Total header size: 216 bytes */
typedef struct {
    char        magic_number[8];        /* Always "MedImag\0" (hex 4d 65 64 49 6d 61 67 00). */
    short       major_version;          /* Currently 1. */
    short       minor_version;          /* Currently 0. */
    short       data_type;              /* See note 2. */
    short       n_dims;                 /* See note 3. */
    short       n_space_dims;           /* See note 3. */
    short       dim_order;              /* See note 3. */
    short       space_unit;             /* See note 4. */
    short       time_unit;              /* See note 4. */
    int         image_dims[6];          /* See note 5. */
    double      voxel_dims[6];          /* See note 4. */
    double      origin[3];              /* See note 4. */
    double      scale_slope;            /* See note 6. */
    double      scale_intercept;        /* See note 6. */
    double      window_min;             /* See note 7. */
    double      window_max;             /* See note 7. */
    double      euler_angles[3];        /* See note 8. */
    short       handedness;             /* See note 8. */
    short       representation;         /* See note 9. */
    short       voxel_count_dim;        /* See note 9. */
    char        description[30];        /* Free text description, using UTF-8 encoding. */
    int         metadata_count;         /* See note 10. */
} miff_header;

/* Total metadata record size: 80 bytes */
typedef struct {
    char        label[20];              /* See note 11. */
    char        value[60];              /* See note 12. */
} miff_metadata;

/* ============================================================================

# NOTES:

1. MIFF files are always stored using big-endian byte ordering.

2. The "data_type" field describes the size and basic type (integer or
    floating-point) of the voxel data stored with the image. Constants
    representing the allowable types are defined below.
    
3. The "n_dims" field gives the overall number of dimensions in the image,
    while "n_space_dims" defines how many of those are spatial (usually 3,
    possibly 2). The "dim_order" field defines the order of the dimensions as
    stored in the file. If this is MIFF_DIMORDER_SPATIAL_FIRST then spatial
    dimensions appear first (as is typical with NIfTI files); if it is
    MIFF_DIMORDER_SPATIAL_LAST then spatial dimensions appear last, so all data
    corresponding to the first voxel appear first, followed by all data from
    the second voxel, and so on.

4. The "space_unit" and "time_unit" fields give the units of the voxel spacings
    (the "voxel_dims" field) along space and time dimensions, respectively.
    Constants representing the allowable units are defined below. The first
    nonspatial dimension is taken to be temporal, while other dimensions have
    no fixed interpretation. The ordering of the "voxel_dims" array should
    respect the "dim_order" field - see note 3. The "origin" field can be used
    to specify a voxel location corresponding to some anatomical reference
    point in the image.

5. The "image_dims" field gives the number of voxels in the image in each
    dimension. A value of zero indicates that the associated dimension is not
    relevant, but zeroes may not appear before the last relevant dimension.
    A 4D image with no time dimension may therefore have "image_dims" of
    (128,128,128,1,16,0), but not (128,128,128,0,16,0). The ordering of this
    array should respect the "dim_order" field - see note 3.

6. The "scale_slope" and "scale_intercept" fields are for data scaling. The
    final voxel values are the stored values multiplied by the slope, plus the
    intercept.

7. The "window_min" and "window_max" fields are hints for the range of the data
    values. If they are not both zero, they may be used by visualisation
    software to set the intensity scale.

8. The basic storage convention for MIFF files is RAS (with "handedness" set to
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

9. The "representation" field indicates exactly what data are stored in the
    file, and in what order. If the encoding is MIFF_REPRESENT_CARTESIAN, every
    voxel's value is stored in sequence, with the first dimension moving
    fastest, and the last dimension moving slowest. If the encoding is
    MIFF_REPRESENT_COORDLIST, the data is divided into blocks. The within-block
    format is a single (32-bit signed) integer, giving the number of nonzero
    voxels in the block, followed by a list of coordinates of nonzero voxels as
    (32-bit signed) integers, followed by a list of their values, using
    whatever data type is indicated by the "data_type" field. Blocks represent
    a single element in the "voxel_count_dim" dimension, perhaps a slice or
    volume. Zero is a valid value for "voxel_count_dim", in which case all
    values are stored in a single block. This representation is potentially
    much more compact for sparse images.

10. The "metadata_count" field gives the number of metadata records which
    follow the mandatory header. This should be zero or greater.

11. Metadata labels consist of UTF-8 encoded free text, and may therefore take
    any value. However, names of the form ".DCM.gggg.eeee", where "gggg" and
    "eeee" each represent four hexadecimal digits, are taken to represent the
    corresponding DICOM tag. (The remaining 6 characters of the label are
    ignored if the first 14 have this format.) Use of this mechanism is highly
    recommended for portability when representing of the (many) quantities for
    which there is a DICOM tag, such as echo time or scan date. The same label
    may appear multiple times, in which case the values would generally be
    concatenated.

12. Metadata values are UTF-8 encoded free text strings. If the corresponding
    label indicates a DICOM tag number (see note 10), then the DICOM
    convention of separating multiple values with a backslash ("\") is
    followed.

============================================================================ */

#define MIFF_DATATYPE_UNDEFINED     0
#define MIFF_DATATYPE_UINT8         1
#define MIFF_DATATYPE_INT16         2
#define MIFF_DATATYPE_INT32         3
#define MIFF_DATATYPE_INT64         4
#define MIFF_DATATYPE_FLOAT32       5
#define MIFF_DATATYPE_FLOAT64       6

#define MIFF_DIMORDER_SPATIAL_FIRST 0
#define MIFF_DIMORDER_SPATIAL_LAST  1

#define MIFF_UNIT_UNDEFINED         0
#define MIFF_UNIT_MICRON            1
#define MIFF_UNIT_MILLIMETRE        2
#define MIFF_UNIT_MILLIMETER        2
#define MIFF_UNIT_METRE             3
#define MIFF_UNIT_METER             3
#define MIFF_UNIT_MICROSECOND      16
#define MIFF_UNIT_MILLISECOND      17
#define MIFF_UNIT_SECOND           18

#define MIFF_REPRESENT_CARTESIAN    0
#define MIFF_REPRESENT_COORDLIST    1

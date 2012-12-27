This project provides a draft definition for the Medical Image File Format (MIFF), an illustrative proposal for a file format intended for containing medical imaging data, with certain desirable characteristics relative to existing commonly-used formats. At present this format is not actually used by any software.

Two important file formats in modern medical imaging are [DICOM](http://dicom.nema.org) and [NIfTI-1](http://nifti.nimh.nih.gov/nifti-1/). The former is widely used on scanner hardware and allows for very rich metadata but is extremely complex. The latter is simple and widely used for interoperability between software packages, but can be inefficient in some contexts.

The key features of MIFF relative to NIfTI-1 are:

- Efficient storage for sparse image data.
- Flexibility in the dimensional ordering of multivariate image data, to enable efficient processing of information voxel-by-voxel.
- Simple free-text metadata, with optional use of DICOM tag codes to identify standard acquisition parameters.
- Simpler conventions with regard to data orientation.
- Fewer voxel data types, to simplify implementation.
- Removal of some redundancy, and of fields which are relatively rarely used.

Comments on the proposed format and its aims are welcome.

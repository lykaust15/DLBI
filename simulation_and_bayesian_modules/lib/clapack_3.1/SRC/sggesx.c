#include "f2c.h"
#include "blaswrap.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c_n1 = -1;
static real c_b42 = 0.f;
static real c_b43 = 1.f;

/* Subroutine */ int sggesx_(char *jobvsl, char *jobvsr, char *sort, L_fp 
	selctg, char *sense, integer *n, real *a, integer *lda, real *b, 
	integer *ldb, integer *sdim, real *alphar, real *alphai, real *beta, 
	real *vsl, integer *ldvsl, real *vsr, integer *ldvsr, real *rconde, 
	real *rcondv, real *work, integer *lwork, integer *iwork, integer *
	liwork, logical *bwork, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, vsl_dim1, vsl_offset, 
	    vsr_dim1, vsr_offset, i__1, i__2;
    real r__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    integer i__, ip;
    real pl, pr, dif[2];
    integer ihi, ilo;
    real eps;
    integer ijob;
    real anrm, bnrm;
    integer ierr, itau, iwrk, lwrk;
    extern logical lsame_(char *, char *);
    integer ileft, icols;
    logical cursl, ilvsl, ilvsr;
    integer irows;
    logical lst2sl;
    extern /* Subroutine */ int slabad_(real *, real *), sggbak_(char *, char 
	    *, integer *, integer *, integer *, real *, real *, integer *, 
	    real *, integer *, integer *), sggbal_(char *, 
	    integer *, real *, integer *, real *, integer *, integer *, 
	    integer *, real *, real *, real *, integer *);
    logical ilascl, ilbscl;
    extern doublereal slamch_(char *), slange_(char *, integer *, 
	    integer *, real *, integer *, real *);
    real safmin;
    extern /* Subroutine */ int sgghrd_(char *, char *, integer *, integer *, 
	    integer *, real *, integer *, real *, integer *, real *, integer *
, real *, integer *, integer *);
    real safmax;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    real bignum;
    extern /* Subroutine */ int slascl_(char *, integer *, integer *, real *, 
	    real *, integer *, integer *, real *, integer *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *);
    integer ijobvl, iright;
    extern /* Subroutine */ int sgeqrf_(integer *, integer *, real *, integer 
	    *, real *, real *, integer *, integer *);
    integer ijobvr;
    extern /* Subroutine */ int slacpy_(char *, integer *, integer *, real *, 
	    integer *, real *, integer *);
    logical wantsb, wantse, lastsl;
    integer liwmin;
    real anrmto, bnrmto;
    integer minwrk, maxwrk;
    logical wantsn;
    real smlnum;
    extern /* Subroutine */ int shgeqz_(char *, char *, char *, integer *, 
	    integer *, integer *, real *, integer *, real *, integer *, real *
, real *, real *, real *, integer *, real *, integer *, real *, 
	    integer *, integer *), slaset_(char *, 
	    integer *, integer *, real *, real *, real *, integer *), 
	    sorgqr_(integer *, integer *, integer *, real *, integer *, real *
, real *, integer *, integer *), stgsen_(integer *, logical *, 
	    logical *, logical *, integer *, real *, integer *, real *, 
	    integer *, real *, real *, real *, real *, integer *, real *, 
	    integer *, integer *, real *, real *, real *, real *, integer *, 
	    integer *, integer *, integer *);
    logical wantst, lquery, wantsv;
    extern /* Subroutine */ int sormqr_(char *, char *, integer *, integer *, 
	    integer *, real *, integer *, real *, real *, integer *, real *, 
	    integer *, integer *);


/*  -- LAPACK driver routine (version 3.1) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Function Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SGGESX computes for a pair of N-by-N real nonsymmetric matrices */
/*  (A,B), the generalized eigenvalues, the real Schur form (S,T), and, */
/*  optionally, the left and/or right matrices of Schur vectors (VSL and */
/*  VSR).  This gives the generalized Schur factorization */

/*       (A,B) = ( (VSL) S (VSR)**T, (VSL) T (VSR)**T ) */

/*  Optionally, it also orders the eigenvalues so that a selected cluster */
/*  of eigenvalues appears in the leading diagonal blocks of the upper */
/*  quasi-triangular matrix S and the upper triangular matrix T; computes */
/*  a reciprocal condition number for the average of the selected */
/*  eigenvalues (RCONDE); and computes a reciprocal condition number for */
/*  the right and left deflating subspaces corresponding to the selected */
/*  eigenvalues (RCONDV). The leading columns of VSL and VSR then form */
/*  an orthonormal basis for the corresponding left and right eigenspaces */
/*  (deflating subspaces). */

/*  A generalized eigenvalue for a pair of matrices (A,B) is a scalar w */
/*  or a ratio alpha/beta = w, such that  A - w*B is singular.  It is */
/*  usually represented as the pair (alpha,beta), as there is a */
/*  reasonable interpretation for beta=0 or for both being zero. */

/*  A pair of matrices (S,T) is in generalized real Schur form if T is */
/*  upper triangular with non-negative diagonal and S is block upper */
/*  triangular with 1-by-1 and 2-by-2 blocks.  1-by-1 blocks correspond */
/*  to real generalized eigenvalues, while 2-by-2 blocks of S will be */
/*  "standardized" by making the corresponding elements of T have the */
/*  form: */
/*          [  a  0  ] */
/*          [  0  b  ] */

/*  and the pair of corresponding 2-by-2 blocks in S and T will have a */
/*  complex conjugate pair of generalized eigenvalues. */


/*  Arguments */
/*  ========= */

/*  JOBVSL  (input) CHARACTER*1 */
/*          = 'N':  do not compute the left Schur vectors; */
/*          = 'V':  compute the left Schur vectors. */

/*  JOBVSR  (input) CHARACTER*1 */
/*          = 'N':  do not compute the right Schur vectors; */
/*          = 'V':  compute the right Schur vectors. */

/*  SORT    (input) CHARACTER*1 */
/*          Specifies whether or not to order the eigenvalues on the */
/*          diagonal of the generalized Schur form. */
/*          = 'N':  Eigenvalues are not ordered; */
/*          = 'S':  Eigenvalues are ordered (see SELCTG). */

/*  SELCTG  (external procedure) LOGICAL FUNCTION of three REAL arguments */
/*          SELCTG must be declared EXTERNAL in the calling subroutine. */
/*          If SORT = 'N', SELCTG is not referenced. */
/*          If SORT = 'S', SELCTG is used to select eigenvalues to sort */
/*          to the top left of the Schur form. */
/*          An eigenvalue (ALPHAR(j)+ALPHAI(j))/BETA(j) is selected if */
/*          SELCTG(ALPHAR(j),ALPHAI(j),BETA(j)) is true; i.e. if either */
/*          one of a complex conjugate pair of eigenvalues is selected, */
/*          then both complex eigenvalues are selected. */
/*          Note that a selected complex eigenvalue may no longer satisfy */
/*          SELCTG(ALPHAR(j),ALPHAI(j),BETA(j)) = .TRUE. after ordering, */
/*          since ordering may change the value of complex eigenvalues */
/*          (especially if the eigenvalue is ill-conditioned), in this */
/*          case INFO is set to N+3. */

/*  SENSE   (input) CHARACTER*1 */
/*          Determines which reciprocal condition numbers are computed. */
/*          = 'N' : None are computed; */
/*          = 'E' : Computed for average of selected eigenvalues only; */
/*          = 'V' : Computed for selected deflating subspaces only; */
/*          = 'B' : Computed for both. */
/*          If SENSE = 'E', 'V', or 'B', SORT must equal 'S'. */

/*  N       (input) INTEGER */
/*          The order of the matrices A, B, VSL, and VSR.  N >= 0. */

/*  A       (input/output) REAL array, dimension (LDA, N) */
/*          On entry, the first of the pair of matrices. */
/*          On exit, A has been overwritten by its generalized Schur */
/*          form S. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of A.  LDA >= max(1,N). */

/*  B       (input/output) REAL array, dimension (LDB, N) */
/*          On entry, the second of the pair of matrices. */
/*          On exit, B has been overwritten by its generalized Schur */
/*          form T. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of B.  LDB >= max(1,N). */

/*  SDIM    (output) INTEGER */
/*          If SORT = 'N', SDIM = 0. */
/*          If SORT = 'S', SDIM = number of eigenvalues (after sorting) */
/*          for which SELCTG is true.  (Complex conjugate pairs for which */
/*          SELCTG is true for either eigenvalue count as 2.) */

/*  ALPHAR  (output) REAL array, dimension (N) */
/*  ALPHAI  (output) REAL array, dimension (N) */
/*  BETA    (output) REAL array, dimension (N) */
/*          On exit, (ALPHAR(j) + ALPHAI(j)*i)/BETA(j), j=1,...,N, will */
/*          be the generalized eigenvalues.  ALPHAR(j) + ALPHAI(j)*i */
/*          and BETA(j),j=1,...,N  are the diagonals of the complex Schur */
/*          form (S,T) that would result if the 2-by-2 diagonal blocks of */
/*          the real Schur form of (A,B) were further reduced to */
/*          triangular form using 2-by-2 complex unitary transformations. */
/*          If ALPHAI(j) is zero, then the j-th eigenvalue is real; if */
/*          positive, then the j-th and (j+1)-st eigenvalues are a */
/*          complex conjugate pair, with ALPHAI(j+1) negative. */

/*          Note: the quotients ALPHAR(j)/BETA(j) and ALPHAI(j)/BETA(j) */
/*          may easily over- or underflow, and BETA(j) may even be zero. */
/*          Thus, the user should avoid naively computing the ratio. */
/*          However, ALPHAR and ALPHAI will be always less than and */
/*          usually comparable with norm(A) in magnitude, and BETA always */
/*          less than and usually comparable with norm(B). */

/*  VSL     (output) REAL array, dimension (LDVSL,N) */
/*          If JOBVSL = 'V', VSL will contain the left Schur vectors. */
/*          Not referenced if JOBVSL = 'N'. */

/*  LDVSL   (input) INTEGER */
/*          The leading dimension of the matrix VSL. LDVSL >=1, and */
/*          if JOBVSL = 'V', LDVSL >= N. */

/*  VSR     (output) REAL array, dimension (LDVSR,N) */
/*          If JOBVSR = 'V', VSR will contain the right Schur vectors. */
/*          Not referenced if JOBVSR = 'N'. */

/*  LDVSR   (input) INTEGER */
/*          The leading dimension of the matrix VSR. LDVSR >= 1, and */
/*          if JOBVSR = 'V', LDVSR >= N. */

/*  RCONDE  (output) REAL array, dimension ( 2 ) */
/*          If SENSE = 'E' or 'B', RCONDE(1) and RCONDE(2) contain the */
/*          reciprocal condition numbers for the average of the selected */
/*          eigenvalues. */
/*          Not referenced if SENSE = 'N' or 'V'. */

/*  RCONDV  (output) REAL array, dimension ( 2 ) */
/*          If SENSE = 'V' or 'B', RCONDV(1) and RCONDV(2) contain the */
/*          reciprocal condition numbers for the selected deflating */
/*          subspaces. */
/*          Not referenced if SENSE = 'N' or 'E'. */

/*  WORK    (workspace/output) REAL array, dimension (MAX(1,LWORK)) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK. */
/*          If N = 0, LWORK >= 1, else if SENSE = 'E', 'V', or 'B', */
/*          LWORK >= max( 8*N, 6*N+16, 2*SDIM*(N-SDIM) ), else */
/*          LWORK >= max( 8*N, 6*N+16 ). */
/*          Note that 2*SDIM*(N-SDIM) <= N*N/2. */
/*          Note also that an error is only returned if */
/*          LWORK < max( 8*N, 6*N+16), but if SENSE = 'E' or 'V' or 'B' */
/*          this may not be large enough. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the bound on the optimal size of the WORK */
/*          array and the minimum size of the IWORK array, returns these */
/*          values as the first entries of the WORK and IWORK arrays, and */
/*          no error message related to LWORK or LIWORK is issued by */
/*          XERBLA. */

/*  IWORK   (workspace) INTEGER array, dimension (MAX(1,LIWORK)) */
/*          On exit, if INFO = 0, IWORK(1) returns the minimum LIWORK. */

/*  LIWORK  (input) INTEGER */
/*          The dimension of the array IWORK. */
/*          If SENSE = 'N' or N = 0, LIWORK >= 1, otherwise */
/*          LIWORK >= N+6. */

/*          If LIWORK = -1, then a workspace query is assumed; the */
/*          routine only calculates the bound on the optimal size of the */
/*          WORK array and the minimum size of the IWORK array, returns */
/*          these values as the first entries of the WORK and IWORK */
/*          arrays, and no error message related to LWORK or LIWORK is */
/*          issued by XERBLA. */

/*  BWORK   (workspace) LOGICAL array, dimension (N) */
/*          Not referenced if SORT = 'N'. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          = 1,...,N: */
/*                The QZ iteration failed.  (A,B) are not in Schur */
/*                form, but ALPHAR(j), ALPHAI(j), and BETA(j) should */
/*                be correct for j=INFO+1,...,N. */
/*          > N:  =N+1: other than QZ iteration failed in SHGEQZ */
/*                =N+2: after reordering, roundoff changed values of */
/*                      some complex eigenvalues so that leading */
/*                      eigenvalues in the Generalized Schur form no */
/*                      longer satisfy SELCTG=.TRUE.  This could also */
/*                      be caused due to scaling. */
/*                =N+3: reordering failed in STGSEN. */

/*  Further details */
/*  =============== */

/*  An approximate (asymptotic) bound on the average absolute error of */
/*  the selected eigenvalues is */

/*       EPS * norm((A, B)) / RCONDE( 1 ). */

/*  An approximate (asymptotic) bound on the maximum angular error in */
/*  the computed deflating subspaces is */

/*       EPS * norm((A, B)) / RCONDV( 2 ). */

/*  See LAPACK User's Guide, section 4.11 for more information. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Decode the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --alphar;
    --alphai;
    --beta;
    vsl_dim1 = *ldvsl;
    vsl_offset = 1 + vsl_dim1;
    vsl -= vsl_offset;
    vsr_dim1 = *ldvsr;
    vsr_offset = 1 + vsr_dim1;
    vsr -= vsr_offset;
    --rconde;
    --rcondv;
    --work;
    --iwork;
    --bwork;

    /* Function Body */
    if (lsame_(jobvsl, "N")) {
	ijobvl = 1;
	ilvsl = FALSE_;
    } else if (lsame_(jobvsl, "V")) {
	ijobvl = 2;
	ilvsl = TRUE_;
    } else {
	ijobvl = -1;
	ilvsl = FALSE_;
    }

    if (lsame_(jobvsr, "N")) {
	ijobvr = 1;
	ilvsr = FALSE_;
    } else if (lsame_(jobvsr, "V")) {
	ijobvr = 2;
	ilvsr = TRUE_;
    } else {
	ijobvr = -1;
	ilvsr = FALSE_;
    }

    wantst = lsame_(sort, "S");
    wantsn = lsame_(sense, "N");
    wantse = lsame_(sense, "E");
    wantsv = lsame_(sense, "V");
    wantsb = lsame_(sense, "B");
    lquery = *lwork == -1 || *liwork == -1;
    if (wantsn) {
	ijob = 0;
    } else if (wantse) {
	ijob = 1;
    } else if (wantsv) {
	ijob = 2;
    } else if (wantsb) {
	ijob = 4;
    }

/*     Test the input arguments */

    *info = 0;
    if (ijobvl <= 0) {
	*info = -1;
    } else if (ijobvr <= 0) {
	*info = -2;
    } else if (! wantst && ! lsame_(sort, "N")) {
	*info = -3;
    } else if (! (wantsn || wantse || wantsv || wantsb) || ! wantst && ! 
	    wantsn) {
	*info = -5;
    } else if (*n < 0) {
	*info = -6;
    } else if (*lda < max(1,*n)) {
	*info = -8;
    } else if (*ldb < max(1,*n)) {
	*info = -10;
    } else if (*ldvsl < 1 || ilvsl && *ldvsl < *n) {
	*info = -16;
    } else if (*ldvsr < 1 || ilvsr && *ldvsr < *n) {
	*info = -18;
    }

/*     Compute workspace */
/*      (Note: Comments in the code beginning "Workspace:" describe the */
/*       minimal amount of workspace needed at that point in the code, */
/*       as well as the preferred amount for good performance. */
/*       NB refers to the optimal block size for the immediately */
/*       following subroutine, as returned by ILAENV.) */

    if (*info == 0) {
	if (*n > 0) {
/* Computing MAX */
	    i__1 = *n << 3, i__2 = *n * 6 + 16;
	    minwrk = max(i__1,i__2);
	    maxwrk = minwrk - *n + *n * ilaenv_(&c__1, "SGEQRF", " ", n, &
		    c__1, n, &c__0);
/* Computing MAX */
	    i__1 = maxwrk, i__2 = minwrk - *n + *n * ilaenv_(&c__1, "SORMQR", 
		    " ", n, &c__1, n, &c_n1);
	    maxwrk = max(i__1,i__2);
	    if (ilvsl) {
/* Computing MAX */
		i__1 = maxwrk, i__2 = minwrk - *n + *n * ilaenv_(&c__1, "SOR"
			"GQR", " ", n, &c__1, n, &c_n1);
		maxwrk = max(i__1,i__2);
	    }
	    lwrk = maxwrk;
	    if (ijob >= 1) {
/* Computing MAX */
		i__1 = lwrk, i__2 = *n * *n / 2;
		lwrk = max(i__1,i__2);
	    }
	} else {
	    minwrk = 1;
	    maxwrk = 1;
	    lwrk = 1;
	}
	work[1] = (real) lwrk;
	if (wantsn || *n == 0) {
	    liwmin = 1;
	} else {
	    liwmin = *n + 6;
	}
	iwork[1] = liwmin;

	if (*lwork < minwrk && ! lquery) {
	    *info = -22;
	} else if (*liwork < liwmin && ! lquery) {
	    *info = -24;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SGGESX", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	*sdim = 0;
	return 0;
    }

/*     Get machine constants */

    eps = slamch_("P");
    safmin = slamch_("S");
    safmax = 1.f / safmin;
    slabad_(&safmin, &safmax);
    smlnum = sqrt(safmin) / eps;
    bignum = 1.f / smlnum;

/*     Scale A if max element outside range [SMLNUM,BIGNUM] */

    anrm = slange_("M", n, n, &a[a_offset], lda, &work[1]);
    ilascl = FALSE_;
    if (anrm > 0.f && anrm < smlnum) {
	anrmto = smlnum;
	ilascl = TRUE_;
    } else if (anrm > bignum) {
	anrmto = bignum;
	ilascl = TRUE_;
    }
    if (ilascl) {
	slascl_("G", &c__0, &c__0, &anrm, &anrmto, n, n, &a[a_offset], lda, &
		ierr);
    }

/*     Scale B if max element outside range [SMLNUM,BIGNUM] */

    bnrm = slange_("M", n, n, &b[b_offset], ldb, &work[1]);
    ilbscl = FALSE_;
    if (bnrm > 0.f && bnrm < smlnum) {
	bnrmto = smlnum;
	ilbscl = TRUE_;
    } else if (bnrm > bignum) {
	bnrmto = bignum;
	ilbscl = TRUE_;
    }
    if (ilbscl) {
	slascl_("G", &c__0, &c__0, &bnrm, &bnrmto, n, n, &b[b_offset], ldb, &
		ierr);
    }

/*     Permute the matrix to make it more nearly triangular */
/*     (Workspace: need 6*N + 2*N for permutation parameters) */

    ileft = 1;
    iright = *n + 1;
    iwrk = iright + *n;
    sggbal_("P", n, &a[a_offset], lda, &b[b_offset], ldb, &ilo, &ihi, &work[
	    ileft], &work[iright], &work[iwrk], &ierr);

/*     Reduce B to triangular form (QR decomposition of B) */
/*     (Workspace: need N, prefer N*NB) */

    irows = ihi + 1 - ilo;
    icols = *n + 1 - ilo;
    itau = iwrk;
    iwrk = itau + irows;
    i__1 = *lwork + 1 - iwrk;
    sgeqrf_(&irows, &icols, &b[ilo + ilo * b_dim1], ldb, &work[itau], &work[
	    iwrk], &i__1, &ierr);

/*     Apply the orthogonal transformation to matrix A */
/*     (Workspace: need N, prefer N*NB) */

    i__1 = *lwork + 1 - iwrk;
    sormqr_("L", "T", &irows, &icols, &irows, &b[ilo + ilo * b_dim1], ldb, &
	    work[itau], &a[ilo + ilo * a_dim1], lda, &work[iwrk], &i__1, &
	    ierr);

/*     Initialize VSL */
/*     (Workspace: need N, prefer N*NB) */

    if (ilvsl) {
	slaset_("Full", n, n, &c_b42, &c_b43, &vsl[vsl_offset], ldvsl);
	if (irows > 1) {
	    i__1 = irows - 1;
	    i__2 = irows - 1;
	    slacpy_("L", &i__1, &i__2, &b[ilo + 1 + ilo * b_dim1], ldb, &vsl[
		    ilo + 1 + ilo * vsl_dim1], ldvsl);
	}
	i__1 = *lwork + 1 - iwrk;
	sorgqr_(&irows, &irows, &irows, &vsl[ilo + ilo * vsl_dim1], ldvsl, &
		work[itau], &work[iwrk], &i__1, &ierr);
    }

/*     Initialize VSR */

    if (ilvsr) {
	slaset_("Full", n, n, &c_b42, &c_b43, &vsr[vsr_offset], ldvsr);
    }

/*     Reduce to generalized Hessenberg form */
/*     (Workspace: none needed) */

    sgghrd_(jobvsl, jobvsr, n, &ilo, &ihi, &a[a_offset], lda, &b[b_offset], 
	    ldb, &vsl[vsl_offset], ldvsl, &vsr[vsr_offset], ldvsr, &ierr);

    *sdim = 0;

/*     Perform QZ algorithm, computing Schur vectors if desired */
/*     (Workspace: need N) */

    iwrk = itau;
    i__1 = *lwork + 1 - iwrk;
    shgeqz_("S", jobvsl, jobvsr, n, &ilo, &ihi, &a[a_offset], lda, &b[
	    b_offset], ldb, &alphar[1], &alphai[1], &beta[1], &vsl[vsl_offset]
, ldvsl, &vsr[vsr_offset], ldvsr, &work[iwrk], &i__1, &ierr);
    if (ierr != 0) {
	if (ierr > 0 && ierr <= *n) {
	    *info = ierr;
	} else if (ierr > *n && ierr <= *n << 1) {
	    *info = ierr - *n;
	} else {
	    *info = *n + 1;
	}
	goto L50;
    }

/*     Sort eigenvalues ALPHA/BETA and compute the reciprocal of */
/*     condition number(s) */
/*     (Workspace: If IJOB >= 1, need MAX( 8*(N+1), 2*SDIM*(N-SDIM) ) */
/*                 otherwise, need 8*(N+1) ) */

    if (wantst) {

/*        Undo scaling on eigenvalues before SELCTGing */

	if (ilascl) {
	    slascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphar[1], 
		    n, &ierr);
	    slascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphai[1], 
		    n, &ierr);
	}
	if (ilbscl) {
	    slascl_("G", &c__0, &c__0, &bnrmto, &bnrm, n, &c__1, &beta[1], n, 
		    &ierr);
	}

/*        Select eigenvalues */

	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    bwork[i__] = (*selctg)(&alphar[i__], &alphai[i__], &beta[i__]);
/* L10: */
	}

/*        Reorder eigenvalues, transform Generalized Schur vectors, and */
/*        compute reciprocal condition numbers */

	i__1 = *lwork - iwrk + 1;
	stgsen_(&ijob, &ilvsl, &ilvsr, &bwork[1], n, &a[a_offset], lda, &b[
		b_offset], ldb, &alphar[1], &alphai[1], &beta[1], &vsl[
		vsl_offset], ldvsl, &vsr[vsr_offset], ldvsr, sdim, &pl, &pr, 
		dif, &work[iwrk], &i__1, &iwork[1], liwork, &ierr);

	if (ijob >= 1) {
/* Computing MAX */
	    i__1 = maxwrk, i__2 = (*sdim << 1) * (*n - *sdim);
	    maxwrk = max(i__1,i__2);
	}
	if (ierr == -22) {

/*            not enough real workspace */

	    *info = -22;
	} else {
	    if (ijob == 1 || ijob == 4) {
		rconde[1] = pl;
		rconde[2] = pr;
	    }
	    if (ijob == 2 || ijob == 4) {
		rcondv[1] = dif[0];
		rcondv[2] = dif[1];
	    }
	    if (ierr == 1) {
		*info = *n + 3;
	    }
	}

    }

/*     Apply permutation to VSL and VSR */
/*     (Workspace: none needed) */

    if (ilvsl) {
	sggbak_("P", "L", n, &ilo, &ihi, &work[ileft], &work[iright], n, &vsl[
		vsl_offset], ldvsl, &ierr);
    }

    if (ilvsr) {
	sggbak_("P", "R", n, &ilo, &ihi, &work[ileft], &work[iright], n, &vsr[
		vsr_offset], ldvsr, &ierr);
    }

/*     Check if unscaling would cause over/underflow, if so, rescale */
/*     (ALPHAR(I),ALPHAI(I),BETA(I)) so BETA(I) is on the order of */
/*     B(I,I) and ALPHAR(I) and ALPHAI(I) are on the order of A(I,I) */

    if (ilascl) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (alphai[i__] != 0.f) {
		if (alphar[i__] / safmax > anrmto / anrm || safmin / alphar[
			i__] > anrm / anrmto) {
		    work[1] = (r__1 = a[i__ + i__ * a_dim1] / alphar[i__], 
			    dabs(r__1));
		    beta[i__] *= work[1];
		    alphar[i__] *= work[1];
		    alphai[i__] *= work[1];
		} else if (alphai[i__] / safmax > anrmto / anrm || safmin / 
			alphai[i__] > anrm / anrmto) {
		    work[1] = (r__1 = a[i__ + (i__ + 1) * a_dim1] / alphai[
			    i__], dabs(r__1));
		    beta[i__] *= work[1];
		    alphar[i__] *= work[1];
		    alphai[i__] *= work[1];
		}
	    }
/* L20: */
	}
    }

    if (ilbscl) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (alphai[i__] != 0.f) {
		if (beta[i__] / safmax > bnrmto / bnrm || safmin / beta[i__] 
			> bnrm / bnrmto) {
		    work[1] = (r__1 = b[i__ + i__ * b_dim1] / beta[i__], dabs(
			    r__1));
		    beta[i__] *= work[1];
		    alphar[i__] *= work[1];
		    alphai[i__] *= work[1];
		}
	    }
/* L25: */
	}
    }

/*     Undo scaling */

    if (ilascl) {
	slascl_("H", &c__0, &c__0, &anrmto, &anrm, n, n, &a[a_offset], lda, &
		ierr);
	slascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphar[1], n, &
		ierr);
	slascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphai[1], n, &
		ierr);
    }

    if (ilbscl) {
	slascl_("U", &c__0, &c__0, &bnrmto, &bnrm, n, n, &b[b_offset], ldb, &
		ierr);
	slascl_("G", &c__0, &c__0, &bnrmto, &bnrm, n, &c__1, &beta[1], n, &
		ierr);
    }

    if (wantst) {

/*        Check if reordering is correct */

	lastsl = TRUE_;
	lst2sl = TRUE_;
	*sdim = 0;
	ip = 0;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    cursl = (*selctg)(&alphar[i__], &alphai[i__], &beta[i__]);
	    if (alphai[i__] == 0.f) {
		if (cursl) {
		    ++(*sdim);
		}
		ip = 0;
		if (cursl && ! lastsl) {
		    *info = *n + 2;
		}
	    } else {
		if (ip == 1) {

/*                 Last eigenvalue of conjugate pair */

		    cursl = cursl || lastsl;
		    lastsl = cursl;
		    if (cursl) {
			*sdim += 2;
		    }
		    ip = -1;
		    if (cursl && ! lst2sl) {
			*info = *n + 2;
		    }
		} else {

/*                 First eigenvalue of conjugate pair */

		    ip = 1;
		}
	    }
	    lst2sl = lastsl;
	    lastsl = cursl;
/* L40: */
	}

    }

L50:

    work[1] = (real) maxwrk;
    iwork[1] = liwmin;

    return 0;

/*     End of SGGESX */

} /* sggesx_ */

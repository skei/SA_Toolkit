

#if 0

struct SAT_Quaternion {
   float x,y,z,w;
};

//----------------------------------------------------------------------
// quaternion
//----------------------------------------------------------------------

// normalize a quaternion

void SAT_Quat_Normalize( float *q ) {
  float l =  sqrt( q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW] );
  if (l==0) {
    q[qW] = 1.0;
    q[qX] = 0.0;
    q[qY] = 0.0;
    q[qZ] = 0.0;
  }
  else {
    float c = 1.0/l;
    q[qX] *= c;
    q[qY] *= c;
    q[qZ] *= c;
    q[qW] *= c;
  }
}

// Note: Order is important! To combine two rotations, use
// the product Mul(qSecond, qFirst), which gives the effect
// of rotation by qFirst, then qSecond.

// Compute quaternion product qL = qL * qR

void SAT_Quat_Mult(float *qL, float *qR) {
  float qp[4];
  qp[qW] = qL[qW]*qR[qW] - qL[qX]*qR[qX] - qL[qY]*qR[qY] - qL[qZ]*qR[qZ];
  qp[qX] = qL[qW]*qR[qX] + qL[qX]*qR[qW] + qL[qY]*qR[qZ] - qL[qZ]*qR[qY];
  qp[qY] = qL[qW]*qR[qY] + qL[qY]*qR[qW] + qL[qZ]*qR[qX] - qL[qX]*qR[qZ];
  qp[qZ] = qL[qW]*qR[qZ] + qL[qZ]*qR[qW] + qL[qX]*qR[qY] - qL[qY]*qR[qX];
  qL[qW] = qp[qW];
  qL[qX] = qp[qX];
  qL[qY] = qp[qY];
  qL[qZ] = qp[qZ];
}

// Form multiplicative inverse of q

void SAT_Quat_Inverse(float *q) {
  float l,norminv;
  l =  (q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW]);
  if (l==0.0) l = 1.0;
  norminv = 1.0/l;
  q[qX] = -q[qX] * norminv;
  q[qY] = -q[qY] * norminv;
  q[qZ] = -q[qZ] * norminv;
  q[qW] =  q[qW] * norminv;
}

// Convert quaternion to 3x3 rotation matrix
// Quaternion need not be unit magnitude.  When it always is,
// this routine can be simplified

void SAT_Quat_ToMatrix(float *q, float *m) {
  float s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz,den;
  // For unit q, just set s = 2.0; or or set xs = q[X] + q[X], etc
  den =  (q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW]);
  if (den==0.0) { s = 1.0; }
  else s = 2.0/den;
  xs = q[qX] * s;   ys = q[qY] * s;  zs = q[qZ] * s;
  wx = q[qW] * xs;  wy = q[qW] * ys; wz = q[qW] * zs;
  xx = q[qX] * xs;  xy = q[qX] * ys; xz = q[qX] * zs;
  yy = q[qY] * ys;  yz = q[qY] * zs; zz = q[qZ] * zs;
  m[ 0] = 1.0 - (yy +zz);
  m[ 1] = xy - wz;
  m[ 2] = xz + wy;
  m[ 4] = xy + wz;
  m[ 5] = 1.0 - (xx +zz);
  m[ 6] = yz - wx;
  m[ 8] = xz - wy;
  m[ 9] = yz + wx;
  m[10] = 1.0 - (xx + yy);
}

// Compute equivalent quaternion from [angle,axis] representation

void SAT_Quat_FromAngAxis(float ang, SAT_Vector3 *axis, float *q) {
  float omega,s;
  omega = ang*0.5;
  s = sin(omega);
  q[qX] = s*axis->x;
  q[qY] = s*axis->y;
  q[qZ] = s*axis->z;
  q[qW] = cos(omega);
}

// Convert quaternion to equivalent [angle,axis] representation

void SAT_Quat_ToAngAxis(float *q, float *ang, SAT_Vector3 *axis) {
  SAT_Quat_Normalize(q);
  float omega = acos(q[3]);
  *ang = 2.0*omega;
  float s = sin(omega);
  if (fabs(s) > .000001) {
    axis->x  = q[qX]/s;
    axis->y  = q[qY]/s;
    axis->z  = q[qZ]/s;
  }
}

// Compute [angle,axis] corresponding to the rotation from
// p to q.  Function returns angle, sets axis.

float SAT_Quat_RelAngAxis(float *p, float *q, SAT_Vector3 *axis) {
  float ang;
  SAT_Quat_Inverse(p);
  SAT_Quat_Mult(p,q);
  SAT_Quat_ToAngAxis(p,&ang,axis);
  return(ang);
}






#endif // 0

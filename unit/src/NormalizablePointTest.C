#include "gtest/gtest.h"

#include "NormalizablePoint.h"

const double tol = 1e-14;

using namespace libMesh;

#define COMMA ,
#define BOUNDS 0 COMMA 10 COMMA 0 COMMA 10 COMMA 0 COMMA 10
#define RANGE -1 COMMA 1 COMMA -1 COMMA 1 COMMA -1 COMMA 1
#define POINTS 1 COMMA 2 COMMA 5
#define NORMALIZED -0.8 COMMA -0.6 COMMA 0

TEST(NormalizablePoint, constructors)
{
  std::vector<Real> bounds = {BOUNDS};
  std::vector<Real> range = {RANGE};
  Point null;
  Point something(POINTS);

  NormalizablePoint no_args;
  EXPECT_EQ(no_args.isNormalized(), false);
  EXPECT_TRUE(null == no_args);

  NormalizablePoint point(something);
  EXPECT_EQ(point.isNormalized(), false);
  EXPECT_TRUE(something == point);

  NormalizablePoint make_points(POINTS);
  EXPECT_EQ(make_points.isNormalized(), false);
  EXPECT_TRUE(something == make_points);

  NormalizablePoint bounds_rvalue({BOUNDS});
  EXPECT_EQ(bounds_rvalue.isNormalized(), false);
  EXPECT_TRUE(null == bounds_rvalue);

  NormalizablePoint bounds_range_rvalue({BOUNDS}, {RANGE});
  EXPECT_EQ(bounds_range_rvalue.isNormalized(), true);
  EXPECT_FALSE(null == bounds_range_rvalue);

  NormalizablePoint bounds_range_rvalue_point({BOUNDS}, {RANGE}, something);
  EXPECT_EQ(bounds_range_rvalue_point.isNormalized(), true);
  EXPECT_FALSE(something == bounds_range_rvalue_point);

  NormalizablePoint bounds_only(bounds);
  EXPECT_EQ(bounds_only.isNormalized(), false);
  EXPECT_TRUE(null == bounds_only);

  NormalizablePoint bounds_range(bounds, range);
  EXPECT_EQ(bounds_range.isNormalized(), true);
  EXPECT_FALSE(null == bounds_range);

  NormalizablePoint bounds_range_point(bounds, range, something);
  EXPECT_EQ(bounds_range_point.isNormalized(), true);
  EXPECT_FALSE(something == bounds_range_point);
}

TEST(NormalizablePoint, mapping)
{
  std::vector<Real> bounds = {BOUNDS};
  std::vector<Real> range = {RANGE};
  Point something(POINTS);
  Point mapped_something(NORMALIZED);
  NormalizablePoint point(bounds, range, something);

  EXPECT_TRUE(mapped_something == point.getNormalized());
  EXPECT_TRUE(something.relative_fuzzy_equals(point.getExpanded(), tol));
  EXPECT_TRUE(something.relative_fuzzy_equals(point.getExpanded(), tol));
  EXPECT_TRUE(mapped_something == point.getNormalized());
}

TEST(NormalizablePoint, constness)
{
  std::vector<Real> bounds = {BOUNDS};
  std::vector<Real> range = {RANGE};
  Point something(POINTS);
  const NormalizablePoint point(bounds, range, something);

  const NormalizablePoint expanded = point.getExpanded();
  const NormalizablePoint normalized = point.getNormalized();
  EXPECT_NE(&point, &expanded);
  EXPECT_NE(&point, &normalized);
}

TEST(NormalizablePoint, set)
{
  std::vector<Real> bounds = {BOUNDS};
  std::vector<Real> range = {RANGE};
  Point something(POINTS);
  Point mapped_something(NORMALIZED);
  NormalizablePoint point(something);
  NormalizablePoint point_rvalue(something);

  point.setBounds(bounds);
  point.setNormalizedRange(range);
  EXPECT_TRUE(mapped_something == point.getNormalized());
  EXPECT_TRUE(mapped_something == point.getNormalized());
  EXPECT_TRUE(something.relative_fuzzy_equals(point.getExpanded(), tol));
  EXPECT_TRUE(something.relative_fuzzy_equals(point.getExpanded(), tol));

  point_rvalue.setBounds({BOUNDS});
  point_rvalue.setNormalizedRange({RANGE});
  EXPECT_TRUE(mapped_something == point_rvalue.getNormalized());
  EXPECT_TRUE(mapped_something == point_rvalue.getNormalized());
  EXPECT_TRUE(something.relative_fuzzy_equals(point.getExpanded(), tol));
  EXPECT_TRUE(something.relative_fuzzy_equals(point.getExpanded(), tol));
}

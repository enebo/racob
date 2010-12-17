/**
 * 
 */
package org.racob.com;

import java.lang.reflect.Array;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.MathContext;
import java.util.Date;

/**
 * A utility class used to convert between Java objects and Variants
 */
public final class VariantUtilities {
    private static Variant[] EMPTY_ARRAY = new Variant[0];

    private VariantUtilities() {
        // utility class with only static methods don't need constructors
    }

    /**
     * Creates a Variant from the supplied Java object
     *
     * @param value
     * @param byRef
     */
    public static Variant createVariant(Object value, boolean byRef) {
        if (value == null) return new Variant();
        if (value instanceof Integer) return new Variant(((Integer) value).intValue(), byRef);
        if (value instanceof Short) return new Variant(((Short) value).shortValue(), byRef);
        if (value instanceof String) return new Variant((String) value, byRef);
        if (value instanceof Boolean) {
//            if (!byRef) {
//                boolean primitiveValue = ((Boolean) value).booleanValue();
//
//                return primitiveValue ? Variant.VT_TRUE : Variant.VT_FALSE;
//            }

            return new Variant((Boolean) value, byRef);
        }
        if (value instanceof Double) return new Variant(((Double) value).doubleValue(), byRef);
        if (value instanceof Float) return new Variant(((Float) value).floatValue(), byRef);
        if (value instanceof BigDecimal) return new Variant(((BigDecimal) value), byRef);
        if (value instanceof Byte) return new Variant(((Byte) value).byteValue(), byRef);
        if (value instanceof Date) return new Variant((Date) value, byRef);
        if (value instanceof Long) return new Variant(((Long) value).intValue(), byRef);
        if (value instanceof Currency) return new Variant((Currency) value, byRef);
        if (value instanceof SafeArray) return new Variant((SafeArray) value, byRef);
        if (value instanceof Dispatch) return new Variant((Dispatch) value, byRef);
        if (value instanceof Variant) return new Variant((Variant) value, byRef);

        // sourceforge patch 2171967 - used to rely on coercion but sometimes crashed VM
        throw new RuntimeException("createVariant() not implemented for " + value.getClass());
    }


    /**
     * Map arguments based on msdn documentation. This method relies on the
     * variant constructor except for arrays.
     *
     * @param objectToBeMadeIntoVariant
     * @return Variant that represents the object
     */
    public static Variant objectToVariant(Object value) {
        if (value == null) return new Variant();
        // if a variant was passed in then be a slacker and just return it
        if (value instanceof Variant) return (Variant) value;
        if (value.getClass().isArray()) {
            // automatically convert arrays using reflection
            // handle it differently based on the type of array
            // added primitive support sourceforge 2762275
            SafeArray sa = null;

            int len1 = Array.getLength(value);
            Class componentType = value.getClass().getComponentType();

            if (componentType.isArray()) { // array of arrays
                sa = new SafeArray(Variant.VariantVariant);

                for (int i = 0; i < len1; i++) {
                    SafeArray subArray = new SafeArray(Variant.VariantVariant);
                    Object e1 = Array.get(value, i);

                    for (int j = 0; j < Array.getLength(e1); j++) {
                        subArray.add(objectToVariant(Array.get(e1, j)));
                    }
                    sa.add(subArray);
                }
            } else if (byte.class.equals(componentType)) {
                byte[] arr = (byte[]) value;
                sa = new SafeArray(Variant.VariantByte);
                for (int i = 0; i < len1; i++) sa.add(i, arr[i]);
            } else if (int.class.equals(componentType)) {
                int[] arr = (int[]) value;
                sa = new SafeArray(Variant.VariantInt);
                for (int i = 0; i < len1; i++) sa.add(i, arr[i]);
            } else if (double.class.equals(componentType)) {
                double[] arr = (double[]) value;
                sa = new SafeArray(Variant.VariantDouble);
                for (int i = 0; i < len1; i++) sa.add(i, arr[i]);
            } else if (long.class.equals(componentType)) {
                long[] arr = (long[]) value;
                sa = new SafeArray(Variant.VariantLongInt);
                for (int i = 0; i < len1; i++) sa.add(i, arr[i]);
            } else { // array of objects
                sa = new SafeArray(Variant.VariantVariant);
                for (int i = 0; i < len1; i++) sa.add(i, objectToVariant(Array.get(value, i)));
            }
            
            return createVariant(sa, false);
        } else {
            // rely on createVariant to throw an exception if its an
            // invalid type
            Variant variant = createVariant(value, false);
//            System.out.println("VARIANT: " + variant.toDebugString() + " for value " + value + " of type " + value.getClass().getSimpleName());
            return variant;
        }
    }

    /**
     * converts an array of objects into an array of Variants by repeatedly
     * calling objectToVariant(Object)
     *
     * @param arrayOfObjectsToBeConverted
     * @return Variant[]
     */
    protected static Variant[] objectsToVariants(Object[] array) {
        if (array.length == 0) return EMPTY_ARRAY;
        
        Variant variants[] = new Variant[array.length];

        for (int i = 0; i < array.length; i++) {
            variants[i] = objectToVariant(array[i]);
        }

        return variants;
    }

    /**
     * Convert a JACOB Variant value to a Java object (type conversions).
     * provided in Sourceforge feature request 959381. A fix was done to handle
     * byRef bug report 1607878.
     * <p>
     * Unlike other toXXX() methods, it does not do a type conversion except for
     * special data types (it shouldn't do any!)
     * <p>
     * Converts Variant.VariantArray types to SafeArrays
     *
     * @return Corresponding Java object of the type matching the Variant type.
     * @throws IllegalStateException
     *             if no underlying windows data structure
     * @throws NotImplementedException
     *             if unsupported conversion is requested
     * @throws JacobException
     *             if the calculated result was a JacobObject usually as a
     *             result of error
     */
    protected static Object variantToObject(Variant sourceData) {
        if (sourceData == null) return null;
        
        short type = sourceData.getType();
        
        if ((type & Variant.VariantArray) == Variant.VariantArray) { // array
            // From SF Bug 1840487
            // This did call toSafeArray(false) but that meant
            // this was the only variantToObject() that didn't have its own
            // copy of the data so you would end up with weird run time
            // errors after some GC. So now we just get stupid about it and
            // always make a copy just like toSafeArray() does.
            return sourceData.getArray();
        } 

        switch (type) {
            case Variant.VariantEmpty: // 0
            case Variant.VariantNull: // 1
                return null;
            case Variant.VariantShort: // 2
                return sourceData.getShort();
            case Variant.VariantUnsignedInt: // 21
            case Variant.VariantInt: // 3
                return sourceData.getInt();
            case Variant.VariantFloat: // 4
                return sourceData.getFloat();
            case Variant.VariantDouble: // 5
                return sourceData.getDouble();
            case Variant.VariantCurrency: // 6
                return sourceData.getCurrency();
            case Variant.VariantDate: // 7
                return sourceData.getDate();
            case Variant.VariantString: // 8
                return sourceData.getString();
            case Variant.VariantDispatch: // 9
                return sourceData.getDispatch();
            case Variant.VariantError: // 10
                throw new RuntimeException("toJavaObject() Not implemented for VariantError");
            case Variant.VariantBoolean: // 11
                return sourceData.getBoolean();
            case Variant.VariantVariant: // 12 they are always by ref
                return sourceData.getVariant();
            case Variant.VariantObject: // 13
                throw new RuntimeException("toJavaObject() Not implemented for VariantObject");
            case Variant.VariantDecimal: // 14
                return sourceData.getDecimal();
            case Variant.VariantByte: // 17
                return new Byte(sourceData.getByte());
            case Variant.VariantUnsignedLong: // 19
            case Variant.VariantLongInt: // 20
                return sourceData.getLong();
            case Variant.VariantTypeMask: // 4095
                throw new RuntimeException("toJavaObject() Not implemented for VariantTypeMask");
            case Variant.VariantArray: // 8192
                throw new RuntimeException("toJavaObject() Not implemented for VariantArray");
            case Variant.VariantByref: // 16384
                throw new RuntimeException("toJavaObject() Not implemented for VariantByref");
            default:
                throw new RuntimeException("Unknown return type: " + type);
                // there was a "return result" here that caused defect 1602118
                // so it was removed
        }
    }

    /**
     * Verifies that we have a scale 0 <= x <= 28 and now more than 96 bits of
     * data. The roundToMSDecimal method will attempt to adjust a BigDecimal to
     * pass this set of tests
     *
     * @param in
     * @throws IllegalArgumentException
     *             if out of bounds
     */
    protected static void validateDecimalScaleAndBits(BigDecimal in) {
        BigInteger allWordBigInt = in.unscaledValue();


        if (in.scale() > 28) {
            // should this cast to a string and call putStringRef()?
            throw new IllegalArgumentException(
                    "VT_DECIMAL only supports a maximum scale of 28 and the passed"
                    + " in value has a scale of " + in.scale());


        } else if (in.scale() < 0) {
            // should this cast to a string and call putStringRef()?
            throw new IllegalArgumentException(
                    "VT_DECIMAL only supports a minimum scale of 0 and the passed"
                    + " in value has a scale of " + in.scale());


        } else if (allWordBigInt.bitLength() > 12 * 8) {
            throw new IllegalArgumentException(
                    "VT_DECIMAL supports a maximum of "
                    + 12
                    * 8
                    + " bits not counting scale and the number passed in has "
                    + allWordBigInt.bitLength());



        } else {
            // no bounds problem to be handled
        }

    }
    /**
     * Largest possible number with scale set to 0
     */
    private static final BigDecimal LARGEST_DECIMAL = new BigDecimal(
            new BigInteger("ffffffffffffffffffffffff", 16));
    /**
     * Smallest possible number with scale set to 0. MS doesn't support negative
     * scales like BigDecimal.
     */
    private static final BigDecimal SMALLEST_DECIMAL = new BigDecimal(
            new BigInteger("ffffffffffffffffffffffff", 16).negate());

    /**
     * Does any validation that couldn't have been fixed by rounding or scale
     * modification.
     *
     * @param in
     *            The BigDecimal to be validated
     * @throws IllegalArgumentException
     *             if the number is too large or too small or null
     */
    protected static void validateDecimalMinMax(BigDecimal in) {
        if (in == null) {
            throw new IllegalArgumentException(
                    "null is not a supported Decimal value.");


        } else if (LARGEST_DECIMAL.compareTo(in) < 0) {
            throw new IllegalArgumentException(
                    "Value too large for VT_DECIMAL data type:" + in.toString()
                    + " integer: " + in.toBigInteger().toString(16)
                    + " scale: " + in.scale());


        } else if (SMALLEST_DECIMAL.compareTo(in) > 0) {
            throw new IllegalArgumentException(
                    "Value too small for VT_DECIMAL data type:" + in.toString()
                    + " integer: " + in.toBigInteger().toString(16)
                    + " scale: " + in.scale());


        }

    }

    /**
     * Rounds the scale and bit length so that it will pass
     * validateDecimalScaleBits(). Developers should call this method if they
     * really want MS Decimal and don't want to lose precision.
     * <p>
     * Changing the scale on a number that can fit in an MS Decimal can change
     * the number's representation enough that it will round to a number too
     * large to be represented by an MS VT_DECIMAL
     *
     * @param sourceDecimal
     * @return BigDecimal a new big decimal that was rounded to fit in an MS
     *         VT_DECIMAL
     */
    public static BigDecimal roundToMSDecimal(BigDecimal sourceDecimal) {
        BigInteger sourceDecimalIntComponent = sourceDecimal.unscaledValue();
        BigDecimal destinationDecimal = new BigDecimal(
                sourceDecimalIntComponent, sourceDecimal.scale());


        int roundingModel = BigDecimal.ROUND_HALF_UP;
        validateDecimalMinMax(
                destinationDecimal);
        // First limit the number of digits and then the precision.
        // Try and round to 29 digits because we can sometimes do that
        BigInteger allWordBigInt;
        allWordBigInt = destinationDecimal.unscaledValue();


        if (allWordBigInt.bitLength() > 96) {
            destinationDecimal = destinationDecimal.round(new MathContext(29));
            // see if 29 digits uses more than 96 bits


            if (allWordBigInt.bitLength() > 96) {
                // Dang. It was over 97 bits so shorten it one more digit to
                // stay <= 96 bits
                destinationDecimal = destinationDecimal.round(new MathContext(
                        28));


            }
        }
        // the bit manipulations above may change the scale so do it afterwards
        // round the scale to the max MS can support
        if (destinationDecimal.scale() > 28) {
            destinationDecimal = destinationDecimal.setScale(28, roundingModel);


        }
        if (destinationDecimal.scale() < 0) {
            destinationDecimal = destinationDecimal.setScale(0, roundingModel);


        }
        return destinationDecimal;

    }
}

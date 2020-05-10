package codingwithmitch.com.databindinggettingstarted.models;

import android.os.Parcel;

import java.math.BigDecimal;


public class Product {

    private static final String TAG = "Product";

    public static final String RED_LAMP_IMAGE                 = "https://images.unsplash.com/photo-1555488205-d5e67846cf40?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1052&q=80";
    public static final String YELLOW_LAMP_IMAGE              = "https://images.unsplash.com/photo-1507668077129-56e32842fceb?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String BLUE_MUG_IMAGE                 = "https://images.unsplash.com/photo-1513127653385-eeaf744da067?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1050&q=80";
    public static final String WHITE_MUG_IMAGE                = "https://images.unsplash.com/photo-1539032760301-ca0ef7fd18d1?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1050&q=80";
    public static final String RED_MUG_IMAGE                  = "https://images.unsplash.com/photo-1541241588155-cb5e3d972891?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1050&q=80";
    public static final String BLACK_HAT_IMAGE                = "https://images.unsplash.com/photo-1565112085863-0b695dca8a3b?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String BLUE_HAT_IMAGE                 = "https://images.unsplash.com/photo-1495298877349-59029703961b?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1489&q=80";
    public static final String WHITE_HAT_IMAGE                = "https://images.unsplash.com/photo-1495264593036-0ffbb3bd000f?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=700&q=80";
    public static final String ORANGE_HAT_IMAGE               = "https://images.unsplash.com/photo-1473286835901-04adb1afab03?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String WHITE_SHIRT_FEMALE_IMAGE       = "https://images.unsplash.com/photo-1516329775056-e78477440820?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String WHITE_SHIRT_MALE_IMAGE         = "https://images.unsplash.com/photo-1519714259805-359338c082bf?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String BLACK_SHIRT_MALE_IMAGE         = "https://images.unsplash.com/photo-1500757810556-5d600d9b737d?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1035&q=80";
    public static final String BLACK_SHIRT_FEMALE_IMAGE       = "https://images.unsplash.com/photo-1518455564870-0771c7b57fa2?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1050&q=80";
    public static final String GREY_FIDGET_SPINNER_IMAGE      = "https://images.unsplash.com/photo-1497040059851-bd928f851c43?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String GREEN_FIDGET_SPINNER_IMAGE     = "https://images.unsplash.com/photo-1497040059851-bd928f851c43?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=634&q=80";
    public static final String ICELAND_PICTURE_IMAGE          = "https://images.unsplash.com/photo-1489593426629-5e4e00ce821c?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=658&q=80";
    public static final String FRANCE_MOUNTAINS_PICTURE_IMAGE = "https://images.unsplash.com/photo-1568304785106-40da9f3706d7?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=967&q=80";
    public static final String GREEN_HILLS_PICTURE_IMAGE      = "https://images.unsplash.com/photo-1531496080470-6e895c9af2ad?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=1500&q=80";
    public static final String HAVASU_FALLS_PICTURE_IMAGE     = "https://images.unsplash.com/photo-1536106034270-e00015d080ae?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=538&q=80";
    public static final String ICEY_COAST_PICTURE_IMAGE       = "https://images.unsplash.com/photo-1559423869-190c0502c8a1?ixlib=rb-1.2.1&ixid=eyJhcHBfaWQiOjEyMDd9&auto=format&fit=crop&w=500&q=60";















    private String title;
    private String description;
    private String image;
    private BigDecimal price;
    private BigDecimal sale_price;
    private int num_ratings;
    private BigDecimal rating;
    private int serial_number;


    public Product(String title, String description, String image, BigDecimal price, BigDecimal sale_price, int num_ratings, BigDecimal rating, int serial_number) {
        this.title = title;
        this.description = description;
        this.image = image;
        this.price = price;
        this.sale_price = sale_price;
        this.num_ratings = num_ratings;
        this.rating = rating;
        this.serial_number = serial_number;
    }

    public Product() {

    }

    protected Product(Parcel in) {
        title = in.readString();
        description = in.readString();
        image = in.readString();
        num_ratings = in.readInt();
        serial_number = in.readInt();
    }


    public boolean hasSalePrice(){
        double salePrice = sale_price.doubleValue();
        if(salePrice > 0) {
            return true;
        }
        else {
            return false;
        }
    }

    public String getNumberRatingsString(){
        return ("(" + getNum_ratings() + ")");
    }

    public static String getTAG() {
        return TAG;
    }

    public int getSerial_number() {
        return serial_number;
    }

    public void setSerial_number(int serial_number) {
        this.serial_number = serial_number;
    }

    public int getNum_ratings() {
        return num_ratings;
    }

    public void setNum_ratings(int num_ratings) {
        this.num_ratings = num_ratings;
    }

    public BigDecimal getRating() {
        return rating;
    }

    public void setRating(BigDecimal rating) {
        this.rating = rating;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getImage() {
        return image;
    }

    public void setImage(String image) {
        this.image = image;
    }

    public BigDecimal getPrice() {
        return price;
    }

    public void setPrice(BigDecimal price) {
        this.price = price;
    }

    public BigDecimal getSale_price() {
        return sale_price;
    }

    public void setSale_price(BigDecimal sale_price) {
        this.sale_price = sale_price;
    }



}

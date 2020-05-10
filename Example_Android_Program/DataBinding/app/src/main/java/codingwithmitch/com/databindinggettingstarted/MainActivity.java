package codingwithmitch.com.databindinggettingstarted;

import android.databinding.DataBindingUtil;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import codingwithmitch.com.databindinggettingstarted.databinding.ActivityMainBinding;
import codingwithmitch.com.databindinggettingstarted.models.Product;
import codingwithmitch.com.databindinggettingstarted.util.Products;

public class MainActivity extends AppCompatActivity implements IMainActivity {

    ActivityMainBinding mBinding;
    private Product mProduct;
    private Products products;
    int index = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       // setContentView(R.layout.activity_main);

        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);

        products = new Products();
        mProduct = products.PRODUCTS [ index ];
        mBinding.setProduct( mProduct );
        mBinding.setQuantity( 1 );
        mBinding.setIMainActivity(this);
        //mBinding.setTesturl("https://external-preview.redd.it/PpC7TRGnLWCUoQZT_pox9W1_hSSaBfmnxJ_y44Kd9ws.jpg?auto=webp&s=ebbfe6f6d6275afc42b7e82acf1fbc0c2ccf869b");
    }


    @Override
    public void inflateQuantityDialog() {

        ChooseQuantityDialog dialog = new ChooseQuantityDialog();
        dialog.show(getSupportFragmentManager(), getString(R.string.dialog_choose_quantity ) );

    }

    @Override
    public void setQuantity(int quantity) {

          mBinding.setQuantity(quantity);
    }

    public void onNextProduct ( View view )
    {

        if ( index < 19) {
            index++;
        }
        else
        {
            index = 0;
        }
        mProduct = products.PRODUCTS[index];
        mBinding.setProduct(mProduct);
    }
}

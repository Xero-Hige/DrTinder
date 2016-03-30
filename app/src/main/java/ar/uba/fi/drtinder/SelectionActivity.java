package ar.uba.fi.drtinder;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.daprlabs.cardstack.SwipeDeck;
import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

/**
 * Swipe selection activity
 */
public class SelectionActivity extends AppCompatActivity {

    //TODO: Remove
    private static final String MAIN_ACTIVITY = "MainActivity";
    private SwipeDeck mCardStack;
    private SwipeDeckAdapter mCardDeckAdapter;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient mClient;

    @Override
    protected void onCreate(Bundle instanceState) {
        super.onCreate(instanceState);
        setContentView(R.layout.activity_selection);
        mCardStack = (SwipeDeck) findViewById(R.id.swipe_deck);

        //TODO: Remove testing data
        ArrayList<String> testData = new ArrayList<>();
        testData.add("Un gato de verdad");
        testData.add("Un gato");
        testData.add("Otro gato");

        List<Integer> resources = new ArrayList<>();
        resources.add(R.drawable.gato_3);
        resources.add(R.drawable.gato_2);
        resources.add(R.drawable.gato_1);
        //TODO: remove testing data

        mCardDeckAdapter = new SwipeDeckAdapter(testData, this, resources);
        mCardStack.setAdapter(mCardDeckAdapter);

        mCardStack.setEventCallback(new SwipeDeck.SwipeEventCallback() {
            //FIXME: remove debug information
            @Override
            public void cardSwipedLeft(int position) {
                Log.i(MAIN_ACTIVITY, "card was swiped left, position in adapter: " + position);
            }

            @Override
            public void cardSwipedRight(int position) {
                Log.i(MAIN_ACTIVITY, "card was swiped right, position in adapter: " + position);
            }

            @Override
            public void cardsDepleted() {
                Log.i(MAIN_ACTIVITY, "no more cards");
            }

            @Override
            public void cardActionDown() {
                Log.i(" ", "cardActionDown");
            }

            @Override
            public void cardActionUp() {
                Log.i(" ", "cardActionUp");
            }

        });
        mCardStack.setLeftImage(R.id.card_nope);
        mCardStack.setRightImage(R.id.card_like);

        Button btn = (Button) findViewById(R.id.nope_button);
        btn.setOnClickListener(view -> mCardStack.swipeTopCardLeft(90));

        Button btn2 = (Button) findViewById(R.id.like_button);
        btn2.setOnClickListener(view -> mCardStack.swipeTopCardRight(90));
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        mClient = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        mClient.connect();
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Selection Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app deep link URI is correct.
                Uri.parse("android-app://ar.uba.fi.drtinder/http/host/path")
        );
        AppIndex.AppIndexApi.start(mClient, viewAction);
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Selection Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app deep link URI is correct.
                Uri.parse("android-app://ar.uba.fi.drtinder/http/host/path")
        );
        AppIndex.AppIndexApi.end(mClient, viewAction);
        mClient.disconnect();
    }

    /**
     * Adapter for swipe deck
     */
    public class SwipeDeckAdapter extends BaseAdapter {

        private List<String> mData;
        private Context mContext;
        private List<Integer> mResources; //Fixme: This should be at mData

        /**
         *
         * @param data Cards data
         * @param context Caller context
         * @param resources TODO: remove this
         */
        public SwipeDeckAdapter(List<String> data, Context context, List<Integer> resources) {
            this.mData = data;
            this.mContext = context;
            this.mResources = resources;
        }

        @Override
        public int getCount() {
            return mData.size();
        }

        @Override
        public Object getItem(int position) {
            return mData.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(final int position, View convertView, ViewGroup parent) {

            View view = convertView;
            if (view == null) {
                LayoutInflater inflater = getLayoutInflater();
                view = inflater.inflate(R.layout.card, parent, false);
            }
            ImageView imageView = (ImageView) view.findViewById(R.id.card_picture);
            Picasso.with(mContext).load(getResourceId(position)).fit().centerCrop().into(imageView);
            TextView textView = (TextView) view.findViewById(R.id.card_text);
            String item = (String) getItem(position);
            textView.setText(item);

            view.setOnClickListener(clickListener -> {
                String item1 = (String) getItem(position);
                Log.i("MainActivity", item1);
            });

            return view;
        }

        private Integer getResourceId(int position) {
            return mResources.get(position % mResources.size());
        }
    }

}
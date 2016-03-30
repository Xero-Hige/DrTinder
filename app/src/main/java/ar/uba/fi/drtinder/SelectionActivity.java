package ar.uba.fi.drtinder;

import android.content.Context;
import android.content.res.ColorStateList;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.daprlabs.cardstack.SwipeDeck;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

/**
 * Swipe selection activity
 */
public class SelectionActivity extends Fragment {

    //TODO: Remove
    private static final String MAIN_ACTIVITY = "MainActivity";
    private SwipeDeck mCardStack;
    private SwipeDeckAdapter mCardDeckAdapter;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.activity_selection, container, false);

        mCardStack = (SwipeDeck) view.findViewById(R.id.swipe_deck);

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

        mCardDeckAdapter = new SwipeDeckAdapter(testData, view.getContext(), resources);
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

        FloatingActionButton btn = (FloatingActionButton) view.findViewById(R.id.nope_button);
        btn.setBackgroundTintList(ColorStateList.valueOf(-1));//FIXME
        btn.setRippleColor(-3355444);//FIXME
        btn.setOnClickListener(newView -> mCardStack.swipeTopCardLeft(90));

        FloatingActionButton btn2 = (FloatingActionButton) view.findViewById(R.id.like_button);
        btn2.setBackgroundTintList(ColorStateList.valueOf(-1));//FIXME
        btn2.setRippleColor(-3355444);//FIXME
        btn2.setOnClickListener(newView -> mCardStack.swipeTopCardRight(90));

        return view;
    }

    @Override
    public void onStart() {
        super.onStart();

    }

    @Override
    public void onStop() {
        super.onStop();
    }

    /**
     * Adapter for swipe deck
     */
    public class SwipeDeckAdapter extends BaseAdapter {

        private List<String> mData;
        private Context mContext;
        private List<Integer> mResources; //Fixme: This should be at mData

        /**
         * @param data      Cards data
         * @param context   Caller context
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
                LayoutInflater inflater = getActivity().getLayoutInflater();
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

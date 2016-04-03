package ar.uba.fi.drtinder;

import android.content.Context;
import android.content.Intent;
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

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

/**
 * Swipe selection activity
 */
public class SelectionActivity extends Fragment {

    //TODO: Remove
    private static final String MAIN_ACTIVITY = "MainActivity";
    private SwipeDeck mCardStack;
    private SwipeDeckAdapter mCardDeckAdapter;
    private boolean show = true;
    private Queue<Map<String, String>> testData;
    private Map<Integer, Map<String, String>> usersdata;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.activity_selection, container, false);

        mCardStack = (SwipeDeck) view.findViewById(R.id.swipe_deck);
        mCardStack.setHardwareAccelerationEnabled(true);

        //TODO: Remove testing data

        testData = new LinkedList<>();
        usersdata = new HashMap<>();

        Map<String, String> cat = new HashMap<>();
        cat.put("name", "Gata1");
        cat.put("age", "21");
        cat.put("img", String.valueOf(R.drawable.gato_1));
        cat.put("bio", "Una gata");

        usersdata.put(0, cat);
        usersdata.put(1, cat);

        testData.add(cat);
        testData.add(cat);

        cat = new HashMap<>();
        cat.put("name", "Gata2");
        cat.put("age", "24");
        cat.put("img", String.valueOf(R.drawable.gato_2));
        cat.put("bio", "Una gata");

        usersdata.put(2, cat);
        testData.add(cat);

        cat = new HashMap<>();
        cat.put("name", "Gata3");
        cat.put("age", "27");
        cat.put("img", String.valueOf(R.drawable.gato_3));
        cat.put("bio", "Una gata");

        usersdata.put(3, cat);
        testData.add(cat);
        //TODO: remove testing data

        mCardDeckAdapter = new SwipeDeckAdapter((view.getContext()));
        mCardStack.setAdapter(mCardDeckAdapter);

        mCardStack.setEventCallback(new SwipeDeck.SwipeEventCallback() {
            //FIXME: remove debug information
            @Override
            public void cardSwipedLeft(int position) {
                Map<String, String> data = testData.remove();
                Snackdebug.showMessage("No te gusto " + data.get("name"), getView());
                show = false;
            }

            @Override
            public void cardSwipedRight(int position) {
                Map<String, String> data = testData.remove();
                Snackdebug.showMessage("Te gustó " + data.get("name"), getView());
                show = false;
            }

            @Override
            public void cardsDepleted() {
                Snackdebug.showMessage("No more cats", getView());

            }

            @Override
            public void cardActionDown() {
                Snackdebug.showMessage("Down", getView());

            }

            @Override
            public void cardActionUp() {
                if (!show) {
                    show = true;
                    return;
                }
                Log.i(" ", "cardActionUp");
                Snackdebug.showMessage("Up", getView());
                Intent menuIntent = new Intent(getContext(), DetailsFragment.class);
                Map<String, String> data = testData.peek();
                menuIntent.putExtra("name", data.get("name"));
                menuIntent.putExtra("age", data.get("age"));
                menuIntent.putExtra("img", data.get("img"));
                menuIntent.putExtra("bio", data.get("bio"));
                startActivity(menuIntent);
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

        private Context mContext;

        /**
         * @param context   Caller context
         */
        public SwipeDeckAdapter(Context context) {

            this.mContext = context;
        }

        @Override
        public int getCount() {
            return usersdata.size();
        }

        @Override
        public Object getItem(int position) {
            return usersdata.get(position).get("name");
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

            ViewGroup.LayoutParams layoutParams = view.getLayoutParams();
            layoutParams.height = (int) getActivity().getResources().getDimension(R.dimen.activity_horizontal_margin);
            view.setLayoutParams(layoutParams);

            ImageView imageView = (ImageView) view.findViewById(R.id.card_picture);
            Picasso.with(mContext).load(getResourceId(position)).fit().centerCrop().into(imageView);
            TextView textView = (TextView) view.findViewById(R.id.card_text);
            String item = usersdata.get(position).get("name") + " , '" + usersdata.get(position).get("age") + "'";
            textView.setText(item);

            view.setOnClickListener(clickListener -> {
                String item1 = (String) getItem(position);
                Log.i("MainActivity", item1);
            });

            return view;
        }

        private Integer getResourceId(int position) {
            return Integer.parseInt(usersdata.get(position).get("img"));
        }
    }

}

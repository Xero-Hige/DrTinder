package ar.uba.fi.drtinder;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.squareup.picasso.Picasso;

public class ChatFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        View view = inflater.inflate(R.layout.chat_fragment, container, false);


        LinearLayout bar = (LinearLayout) view.findViewById(R.id.chat_item);
        for (int i = 0; i < 20; i = i * 2) {
            View layout = inflater.inflate(R.layout.chat_user_layout_left, null);
            TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
            nameTextView.setText("Gata " + i);
            TextView ageTextView = (TextView) layout.findViewById(R.id.chat_user_age);
            ageTextView.setText("(" + ((127 * i) % 19 + 20) + ")");
            ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.gato_3).fit().centerCrop().into(imageView);

            bar.addView(layout);

            layout = inflater.inflate(R.layout.chat_user_layout_right, null);
            nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
            nameTextView.setText("Gata " + (i + 1));
            ageTextView = (TextView) layout.findViewById(R.id.chat_user_age);
            ageTextView.setText("(" + ((341 * i) % 19 + 20) + ")");
            imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.gato_4).fit().centerCrop().into(imageView);

            bar.addView(layout);
        }

        return view;
    }
}

package ar.uba.fi.drtinder;

import android.content.Intent;
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

/**
 * Chat list fragment
 */
public class ChatFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        View view = inflater.inflate(R.layout.chat_fragment, container, false);


        LinearLayout bar = (LinearLayout) view.findViewById(R.id.chat_item);
        for (int i = 1; i < 21; i *= 2) {
            View layout = inflater.inflate(R.layout.chat_user_layout_left, null);
            TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
            nameTextView.setText("Gata " + i);
            TextView ageTextView = (TextView) layout.findViewById(R.id.chat_user_age);
            ageTextView.setText("(" + ((127 * i) % 19 + 20) + ")");
            ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.gato_5).fit().centerCrop().into(imageView);

            final int finalI = i;
            imageView.setOnClickListener(listener -> {
                Intent menuIntent = new Intent(container.getContext(), ChatSession.class);
                menuIntent.putExtra("User", "Anastasia Steel" + finalI);
                menuIntent.putExtra("ID", "burno");
                startActivity(menuIntent);
            });


            bar.addView(layout);

            layout = inflater.inflate(R.layout.chat_user_layout_right, null);
            nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
            nameTextView.setText("Gata " + (i + 1));
            ageTextView = (TextView) layout.findViewById(R.id.chat_user_age);
            ageTextView.setText("(" + ((341 * i) % 19 + 20) + ")");
            imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.gato_5).fit().centerCrop().into(imageView);

            imageView.setOnClickListener(listener -> {
                Intent menuIntent = new Intent(container.getContext(), ChatSession.class);
                menuIntent.putExtra("User", "Anastasia Steel" + finalI);
                menuIntent.putExtra("ID", "burno");
                startActivity(menuIntent);
            });

            bar.addView(layout);
        }

        return view;
    }
}

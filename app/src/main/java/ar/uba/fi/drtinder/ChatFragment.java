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

/**
 * Chat list fragment
 */
public class ChatFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        View view = inflater.inflate(R.layout.chat_fragment, container, false);

        LinearLayout bar = (LinearLayout) view.findViewById(R.id.chat_item);

        StringResourcesHandler.executeQuery("christianGray", StringResourcesHandler.USER_MATCHES,
                data -> {
                    for (int i = 0; i < data.size(); i++) {
                        String name = data.get(i)[0];
                        String age = data.get(i)[1];
                        String user = data.get(i)[2];

                        View layout = inflater.inflate(R.layout.chat_user_layout_left, container, false);
                        TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
                        nameTextView.setText(name);
                        TextView ageTextView = (TextView) layout.findViewById(R.id.chat_user_age);
                        ageTextView.setText(age);
                        ImageView imageView = (ImageView) layout.findViewById(R.id.chat_user_img);
                        ImageResourcesHandler.fillImageResource(user, ImageResourcesHandler.RES_USER_IMG,
                                imageView, getContext());

                        imageView.setOnClickListener(listener -> {
                            Intent menuIntent = new Intent(container.getContext(), ChatSession.class);
                            menuIntent.putExtra("User", name);
                            menuIntent.putExtra("ID", user);
                            startActivity(menuIntent);
                        });
                        bar.addView(layout);
                    }
                });


        return view;
    }
}

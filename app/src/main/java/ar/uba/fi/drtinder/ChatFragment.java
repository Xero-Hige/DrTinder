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
 * @author Xero-Hige
 * Copyright 2016 Gaston Martinez Gaston.martinez.90@gmail.com
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 * Chat list fragment
 */
public class ChatFragment extends Fragment {

    /**
     * Called to have the fragment instantiate its user interface view.
     *
     * @param inflater  The LayoutInflater object that can be used to inflate any views in the fragment
     * @param container If non-null, this is the parent view that the fragment's UI should be
     *                  attached to. The fragment should not add the view itself, but this can
     *                  be used to generate the LayoutParams of the view.
     * @param bundle    If non-null, this fragment is being re-constructed from a previous saved
     *                  state as given here.
     * @return Return the View for the fragment's UI, or null.
     */
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        View view = inflater.inflate(R.layout.chat_fragment, container, false);

        LinearLayout bar = (LinearLayout) view.findViewById(R.id.chat_item);

        StringResourcesHandler.executeQuery(StringResourcesHandler.USER_MATCHES,
                UserHandler.getToken(),
                data -> {
                    if (data == null) {
                        Utility.showMessage("Error de conexion con el servidor", getView(), "Ok");
                        return;
                    }

                    for (int i = 0; i < data.size(); i++) {
                        String name = data.get(i)[0];
                        String age = data.get(i)[1];
                        String user = data.get(i)[2];

                        View layout = inflater.inflate(R.layout.chat_user_layout_left,
                                container, false);
                        ImageView imageView = addUserChat(name, age, user, layout);

                        imageView.setOnClickListener(listener -> {
                            assert container != null; //DEBUG Assert
                            Intent menuIntent = new Intent(container.getContext(),
                                    ChatSession.class);
                            menuIntent.putExtra("User", name);
                            menuIntent.putExtra("ID", user);
                            startActivity(menuIntent);
                        });
                        bar.addView(layout);
                    }
                });


        return view;
    }

    private ImageView addUserChat(String name, String age, String username, View chatLayout) {
        TextView nameTextView = (TextView) chatLayout.findViewById(R.id.chat_user_name);
        nameTextView.setText(name);
        TextView ageTextView = (TextView) chatLayout.findViewById(R.id.chat_user_age);
        ageTextView.setText(age);
        ImageView imageView = (ImageView) chatLayout.findViewById(R.id.chat_user_img);
        ImageResourcesHandler.fillImageResource(username, ImageResourcesHandler.RES_USER_IMG,
                UserHandler.getToken(), imageView, getContext());
        return imageView;
    }
}

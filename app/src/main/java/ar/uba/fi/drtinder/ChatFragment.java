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
        for (int i = 9; i > -1; i--) {
            View layout = inflater.inflate(R.layout.interest_lay, null);
            TextView textView = (TextView) layout.findViewById(R.id.interst_txt);
            textView.setText("Interes: " + i);
            ImageView imageView = (ImageView) layout.findViewById(R.id.interst_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.ubuntu).fit().centerCrop().into(imageView);
            bar.addView(layout);

            layout = inflater.inflate(R.layout.interest_lay, null);
            textView = (TextView) layout.findViewById(R.id.interst_txt);
            textView.setText("Interes: " + i);
            imageView = (ImageView) layout.findViewById(R.id.interst_img);
            Picasso.with(bar.getContext()).load(R.drawable.logo).fit().centerCrop().into(imageView);
            bar.addView(layout);
        }

        return view;
    }
}

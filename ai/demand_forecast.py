import re
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.preprocessing import LabelEncoder
from datetime import datetime

DATA_FILE = "data/demand_history.csv"


def train_model():
    """Load demand history and train a passenger-demand model."""
    df = pd.read_csv(DATA_FILE)

    required_columns = {
        "date", "stop_id", "time_period", "passenger_count"
    }

    if not required_columns.issubset(df.columns):
        raise ValueError("Demand CSV is missing required columns.")

    if df.empty:
        raise ValueError("Demand history is empty.")

    if df["stop_id"].isna().any():
        raise ValueError("Stop IDs cannot be empty.")

    if df["passenger_count"].isna().any():
        raise ValueError("Passenger counts cannot be empty.")

    if (df["passenger_count"] < 0).any():
        raise ValueError("Passenger counts cannot be negative.")

    df["date"] = pd.to_datetime(df["date"], errors="raise")
    df["day_of_week"] = df["date"].dt.dayofweek

    encoder = LabelEncoder()
    df["time_code"] = encoder.fit_transform(
        df["time_period"].astype(str).str.lower().str.strip()
    )

    features = ["stop_id", "day_of_week", "time_code"]
    X = df[features]
    y = df["passenger_count"]

    model = RandomForestRegressor(
        n_estimators=100,
        random_state=42
    )
    model.fit(X, y)

    return model, encoder, df


def predict_demand(stop_id, date, time_period):
    """Predict passenger demand for a stop, date, and time period."""
    model, encoder, df = train_model()

    if stop_id not in df["stop_id"].unique():
        raise ValueError(
            "Invalid stop ID. Use a stop present in the demand dataset."
        )

    time_period = time_period.lower().strip()

    if time_period not in encoder.classes_:
        allowed = ", ".join(encoder.classes_)
        raise ValueError(f"Unknown time period. Use: {allowed}")

    try:
        date = date.strip()

        if not re.fullmatch(r"\d{4}-\d{2}-\d{2}", date):
            raise ValueError

        prediction_date = datetime.strptime(date, "%Y-%m-%d")

    except ValueError:
        raise ValueError(
            "Invalid date. Enter a real date in YYYY-MM-DD format."
        )

    if not 2020 <= prediction_date.year <= 2035:
        raise ValueError("Year must be between 2020 and 2035.")

    day_of_week = prediction_date.dayofweek
    time_code = encoder.transform([time_period])[0]

    features = pd.DataFrame([{
        "stop_id": stop_id,
        "day_of_week": day_of_week,
        "time_code": time_code
    }])

    prediction = model.predict(features)[0]
    return max(0, round(prediction))


if __name__ == "__main__":
    try:
        stop = int(input("Enter stop ID: "))
        date = input("Enter prediction date (YYYY-MM-DD): ")
        period = input(
            "Enter time period (morning/afternoon/evening): "
        )

        result = predict_demand(stop, date, period)

        print("\n--- Passenger Demand Forecast ---")
        print("Stop ID:", stop)
        print("Date:", date)
        print("Time period:", period)
        print("Predicted passengers:", result)
        print("\nNote: This model uses simulated sample data.")

    except (ValueError, FileNotFoundError) as error:
        print("Forecast error:", error)

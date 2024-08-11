
import { useEffect, useState } from 'react';
import './scss/App.css';

function App() {
  const [access, setAccess] = useState("SWIPE YOUR CARD")
  const [UID, setUID] = useState("")
  const [key, setKey] = useState("default")

  setInterval(() => {
    fetch('http://localhost:8000/getCard')
      .then(res => res.json())
      .then(data => {
        setUID(data.UID);
        setKey(data.key);
        setAccess(data.access);
      })
      .catch((err) => console.log(err))
  }, 100)

  return (
    <div className="App">
      <div className='wrapper'>
        <header>
          <h1>SMART TỦ</h1>
        </header>
        <div className='access'>
          <h4 className={key === "access" ? "access-UID " : key === "denied" ? "denied-UID " : ""}>{access}</h4>
        </div>
        <div className='tu'>
          <div className='display'>
            <p className={key === "access" ? "access-UID " : key === "denied" ? "denied-UID " : ""}>{UID}</p>
          </div>
          <div className={key === "access" ? "access-key key" : key === "denied" ? " denied-key key" : "default key"}></div>
        </div>
      </div>
    </div>
  );
}

export default App;

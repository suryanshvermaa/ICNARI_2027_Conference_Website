import { useEffect, useState } from "react";
import axios from "axios";
import { useNavigate } from "react-router-dom";

const PaperUpdates = () => {
  const [papers, setPapers] = useState([]);    // Store all fetched papers
  const [updates, setUpdates] = useState([]);   // Store all fetched updates
  const [visiblePapers, setVisiblePapers] = useState([]); // Store only 15 visible papers
  const [visibleUpdates, setVisibleUpdates] = useState([]); // Store only 15 visible updates
  const [loading, setLoading] = useState(false); // Loading state
    const navigate=useNavigate();

  const splitPaperDescription = (description) => {
    const text = String(description || "");
    const parts = text.split(/\n\nAuthors:\s*/i);
    return {
      authors: (parts[1] || "").trim(),
    };
  };

  const stripEventDateLine = (description) => {
    if (!description) return "";
    return String(description)
      .replace(/\n\nEvent Date:.*$/ms, "")
      .trim();
  };

  // Fetch all papers and updates from API
  const fetchPapersAndUpdates = async () => {
    setLoading(true);
    try {
      // Fetch all papers
      const papersResponse = await axios.get(`${import.meta.env.VITE_API_URL}/api/v1/notifications`, {
        params: { type: "HIGHLIGHTS", page: 1, limit: 1000 },
      });
      // Fetch all updates
      const updatesResponse = await axios.get(`${import.meta.env.VITE_API_URL}/api/v1/notifications`, {
        params: { type: "RECENT_UPDATES", page: 1, limit: 1000 },
      });
      
      // Store all papers and updates
      const allPapers = papersResponse?.data?.data ?? [];
      const allUpdates = updatesResponse?.data?.data ?? [];
      setPapers(allPapers);
      setUpdates(allUpdates);
      
      // Set the first 15 items to be visible
      setVisiblePapers(allPapers.slice(0, 15));
      setVisibleUpdates(allUpdates.slice(0, 15));

    } catch (error) {
      console.error("Error fetching data:", error);
    }
    setLoading(false);
  };

  useEffect(() => {
    fetchPapersAndUpdates();
  }, []);

  return (
    <div className="container mx-auto px-4 py-8">
      <div className="flex flex-col md:flex-row gap-6">

        {/* 📄 Papers Received */}
        <div className="site-card p-6 flex-1 h-[60%] overflow-hidden">
          <h2 className="text-2xl font-semibold text-zinc-900 dark:text-slate-50 mb-4 border-b border-zinc-200 pb-2 dark:border-slate-700/60">📄 Highlights</h2>
          <ul className="space-y-3 max-h-[500px] overflow-y-auto">
            {visiblePapers.map((paper, index) => (
              <li key={index} className="bg-indigo-50 dark:bg-indigo-500/10 p-4 rounded-md shadow-sm flex justify-between items-center transition-transform duration-300">
                <div>
                  <strong className="text-zinc-900 dark:text-slate-50">{paper.title}</strong>
                  <p className="text-zinc-600 dark:text-slate-300 text-sm font-bold">Authors: {splitPaperDescription(paper.description).authors || "—"}</p>
                </div>
                {paper.link && (
                  <a href={paper.link} target="_blank" rel="noopener noreferrer">
                    <button className="bg-indigo-600 text-white px-4 py-2 rounded-md hover:bg-indigo-500 dark:bg-indigo-500 dark:hover:bg-indigo-400 transition duration-300">
                      View
                    </button>
                  </a>
                )}
              </li>
            ))}
          </ul>
          {papers.length > visiblePapers.length && (
            <button onClick={()=>navigate("/allpapers")} className="mt-4 bg-indigo-600 text-white px-6 py-2 rounded-md hover:bg-indigo-500 dark:bg-indigo-500 dark:hover:bg-indigo-400 transition duration-300">
              View more
            </button>
          )}
        </div>

        {/* 🔥 Recent Updates */}
        <div className="site-card p-6 flex-1 overflow-hidden">
          <h2 className="text-2xl font-semibold text-zinc-900 dark:text-slate-50 mb-4 border-b border-zinc-200 pb-2 dark:border-slate-700/60">🔥 Recent Updates</h2>
          <ul className="space-y-3 max-h-[500px] overflow-y-auto">
            {visibleUpdates.map((update, index) => (
              <li key={index} className="bg-indigo-50 dark:bg-indigo-500/10 gap-1 p-4 rounded-md shadow-sm flex justify-between items-center transition-transform duration-300">
                <div>
                  <strong className="text-zinc-900 dark:text-slate-50">{update.title}</strong>
                  <p className="text-zinc-600 dark:text-slate-300 text-sm font-bold">{stripEventDateLine(update.description)}</p>
                </div>
                {update.link && (
                  <a href={update.link} target="_blank" rel="noopener noreferrer">
                    <button className="bg-indigo-600 text-white px-4 py-2 rounded-md hover:bg-indigo-500 dark:bg-indigo-500 dark:hover:bg-indigo-400 transition duration-300">
                      View
                    </button>
                  </a>
                )}
              </li>
            ))}
          </ul>
          {updates.length > visibleUpdates.length && (
            <button onClick={()=>navigate("/allupdates")} className="mt-4 bg-indigo-600 text-white px-6 py-2 rounded-md hover:bg-indigo-500 dark:bg-indigo-500 dark:hover:bg-indigo-400 transition duration-300">
            View more
          </button>
          )}
        </div>
      </div>

      {/* Loading Indicator */}
      {loading && <p className="text-center text-zinc-500 dark:text-slate-300 mt-4">Loading...</p>}
    </div>
  );
};

export default PaperUpdates;
